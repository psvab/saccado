#pragma once

#include <unordered_map>
#include <cassert>
#include "General.h"
#include "Cache.h"
#include "Logger.h"
#include "OrderOp.h"
#include "MData.h"

namespace SC
{

   
    
    
struct BookLevel;    
struct BookOrder;

struct ListBase
{
    void list_append(ListBase* bo) 
    {
        bo->_next=_next;
        bo->_prev=this;
        _next->_prev = bo;
        _next = bo;
    }
    void list_remove()
    {
        _next->_prev = _prev;
        _prev->_next = _next;
    }
    void list_reset()
    {
        _prev = this;
        _next = this;    
    }
    
    template<typename T> T* prev() { return static_cast<T*>(_prev); }
    template<typename T> T* next() { return static_cast<T*>(_next); }
  
    ListBase *_prev{this}, *_next{this};
};

struct BookOrder : ListBase
{
    void set(uint64_t id, int qty, BookLevel* level) 
    { 
        _id=id, _qty=qty,_level=level; 
    }
    uint64_t _id{0};
    int      _qty{0};
    BookLevel* _level{nullptr};    
};
    
typedef Cache<BookOrder> BookOrderCache;


struct BookLevel : ListBase
{    
    BookLevel(eBuySell side_ = SideCount) : _side(side_)
    {}
    void add(BookOrder* bo)
    {
        assert(bo->_qty);
        _head.list_append(bo);
        _qty += bo->_qty;
    }
    
    void set(double price, int qty)
    {
        _price = price; _qty = qty;
        list_reset();
    }
       
    
    double _price{0};
    int _qty{0};
    eBuySell _side{SideCount};
    BookOrder _head;
};
    
typedef std::unordered_map<uint64_t, BookOrder*> BookOrderMap;

// Class handling one side of a book
// Simple update mechanism where we can lookup orders by ID and levels by price
// Orders are stored on a level as a double linked list and levels are
//   also double linked. The longest operation is finding where to insert 
// new price level TODO: .. this should be optimised 
template<eBuySell Side>
class BookSide
{
public:
    BookSide(BookOrderCache& cache, BookOrderMap& order_map) : 
        _order_by_id(order_map),
        _level_cache(65536,Side), 
        _bo_cache(cache)
        
    {
        _head._price = BuySellTraits<Side>::worst_price;
        _best = &_head;
    }
        
    const BookLevel& best() const { return *_best; }
        
        
    void add(const OrderOpBase& op)
    {
        BookLevel* level{nullptr};
        auto loc = _level_by_price.find(op._price);
        if(loc==_level_by_price.end())
        {
            level = &_level_cache.get();
            level->set(op._price,0);
            
            BookLevel* it = _best;
            while( BuySellTraits<Side>::better(it->_price, level->_price) ) // This could get slow, book keeping logic to be optimised
                it = it->next<BookLevel>();
            it->_prev->list_append(level);
                
            
            if( BuySellTraits<Side>::better(level->_price, _best->_price) )
                _best=level;
            
            _level_by_price[level->_price] = level;
        }else
            level = loc->second;
        
        BookOrder& bo = _bo_cache.get();
        bo.set(op._order_id, op._qty, level);
        _order_by_id[op._order_id] = &bo;
        level->add(&bo);
    }
    
    void remove(const OrderOpBase& op)
    {
        BookOrder * order = _order_by_id[op._order_id];
        if(!order)
        {
            LOG_ERROR("Order not found in a book : " << op._order_id);
            return;
        }
        remove(order);
    }
    
    void remove(BookOrder* order)
    {
        order->list_remove();
        
        auto level = order->_level;
        level->_qty -= order->_qty;
        if(level->_qty<=0)
            remove_level(*level);
    }
    
    void trade(const OrderOpBase& op)
    {
        BookOrder * order = _order_by_id[op._order_id];
        if(!order)
        {
            LOG_ERROR("Order not found in a book : " << op._order_id);
            return;
        }
        trade(order, op._qty);
    }
    void trade(BookOrder* order, int qty)
    {
        order->_qty -= qty;
        order->_level->_qty -= qty;
        if(order->_qty <=0 )
            remove(order);
    }
    
    void remove_level(BookLevel& level)
    {
        if( _best == &level )
        {
            if( BuySellTraits<Side>::better(level.prev<BookLevel>()->_price,
                                            level.next<BookLevel>()->_price) )
                _best = level.prev<BookLevel>();
            else
                _best = level.next<BookLevel>();
        }
        _level_by_price.erase(level._price);
        level.list_remove();
        _level_cache.give_back(level);
    }
private:
    BookLevel *_best;
    BookLevel _head;
    
    std::unordered_map<Price, BookLevel*> _level_by_price;
    BookOrderMap& _order_by_id;
    Cache<BookLevel> _level_cache;
    BookOrderCache   _bo_cache;
};
    
// Order book of one instrument    
 class Book
 {
 public:
     Book(InstrumentId inst) : 
        _instId(inst),
        _bo_cache(1048576), 
        _buy_side(_bo_cache, _order_by_id), 
        _sell_side(_bo_cache, _order_by_id)
    {
            _latest._instId = _instId;
    }
     
    void apply_op(const OrderOp& op)
    {         
        _last_update = op._time_recv;
        _last_time_exchg = op._time;
        switch(op._op)
        {
        case OpOrderNew:
            if( op._side==Buy ) _buy_side.add(op) ;
            else _sell_side.add(op) ;
            break;
        case OpOrderCancel:
            if( op._side==Buy ) _buy_side.remove(op) ;
            else _sell_side.remove(op) ;
            break;
        case OpOrderTrade:
            {
                BookOrder * order = _order_by_id[op._order_id];
                if(!order)
                {
                    LOG_ERROR("Order not found in a book : " << op._order_id);
                    return;
                }
                if( order->_level->_side == Buy )
                    _buy_side.trade(order, op._qty);
                else
                    _sell_side.trade(order, op._qty);
            }
            break;
        default:
            throw std::runtime_error("Unexpected order op");
        }
        //TODO: optimise with dirty flag
        update_md();
    }
     
    //the latest market data we have
    const MData& latest() const { return _latest; }
    // return true iff changed with the latest update
    bool changed() const { return _changed; }
     
 private:
     
    void update_md()
    {               
        BidAskVals new_quote{_buy_side.best()._price, _sell_side.best()._price,
                _buy_side.best()._qty, _sell_side.best()._qty };
        if( _latest._quote != new_quote )
        {
            _latest._quote = new_quote;
            _latest._time = _last_update;
            _latest._time_exchg = _last_time_exchg;
            _changed = true;
        }else
            _changed=false;
    }
     
     
    InstrumentId    _instId{0};
    IntTime        _last_update{0};
    IntTime        _last_time_exchg{0};
    MData          _latest;
    bool           _changed{false};
    BookOrderMap _order_by_id;
    BookOrderCache _bo_cache;
    BookSide<Buy> _buy_side;
    BookSide<Sell> _sell_side;     
 };
    
    
    
    
}

