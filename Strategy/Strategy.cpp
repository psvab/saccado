#include "Strategy.h"
#include "Instref.h"
#include "ComponentManager.h"
#include "EventQ.h"

namespace SC
{

   
    
void Strategy1::load(const ptree& config)
{
    _max_orders = config.get<int>("max_orders");
    _max_position = config.get<int>("max_pos");
    
    vector<TradingLink*> tlinks = ComponentManager::instance().get_all<TradingLink>();
    const auto& insts = config.get_child("insts");
    for(const auto & i : insts)
    {        
        std::string instid = i.second.data();
        Instrument& inst = Instref::instance().getByExchgIdOrThrow(instid);
        InstInfo& info = _instInfo[inst.id()];
        info._max_orders = _max_orders;
        info._max_position = _max_position;
        info._inst = &inst;        
        for(auto tlink : tlinks)
        {
            if( tlink->handles(inst.id()) )
            {
                info._tlink = tlink;                
                break;
            }
        }
        EXCEPTION_ASSERT(info._tlink, "No tlink found for inst " + inst.exId());
    }
    ComponentManager::instance().get_first<EventQ>().registerListener(*this);
}


void Strategy1::stop() 
{
    ComponentManager::instance().get_first<EventQ>().unregisterListener();
} 


inline void Strategy1::tryOrder(InstInfo& info, Price price, int qty, eBuySell side)
{
    // Only allowed one active order to prevent in-flight volume
    // TODO: better in-flight handling
    if( info._order._status == OrderStatusActive ) 
        return;
    
    if( info._last_time != (_time >> 30) ) // clear counters
    {
        info._sent_qty = 0;
        info._last_time = _time >> 30;        
    }
    
    // check frequency of order volume
    if( info._sent_qty + qty > info._max_orders )        
        return;

    // check position against max
    if( abs(info._position + sideSign(side)*qty) > info._max_position )        
        return;

    IntTime timeBeforeSend = chrono::high_resolution_clock::now().time_since_epoch().count();
    info._tlink->sendOrderNew(info._inst, price, qty, side);    
    IntTime timeSent = chrono::high_resolution_clock::now().time_since_epoch().count();
    
    info._sent_qty += qty;
    info._order.set_new(price, qty, side);
    
    LOG_INFO("ORDER_SENT at " << timeSent << " for " << info._inst->exId() << " "
          << info._inst->id() << ":" << info._order._order_id
          << "  "<< qty << "@" << price << " side "<< side << "  md_time " << _time 
          << " lat " << timeBeforeSend-_time << " " << timeSent-_time);

    ++_orders_sent;
}

inline void Strategy1::recalculate(InstInfo& info, const MData& newMD)
{
    if( newMD._quote.valid() && info._md._quote.valid() )
    {
        if(newMD._quote._bid > info._md._quote._bid)
            tryOrder(info, newMD._quote._bid, 1, Sell);
        if(newMD._quote._ask < info._md._quote._ask)
            tryOrder(info, newMD._quote._ask, 1, Buy);
    }    
}



void Strategy1::handleEvent(const Event& event)
{
    switch(event._base._type)
    {
    case EventTypeMData: 
    {
        const MData& md = event._md._md;
        _time = md._time;
        InstInfo& info = _instInfo[md._instId];
        recalculate(info, md);
        info._md = md;        
        break;
    }
    case EventTypeFill: 
    {
        const Fill& fill = event._fill._fill;
        _time = fill._time;
        InstInfo& info = _instInfo[fill._inst];        
        info._position += sideSign(fill._side) * fill._qty;        
        LOG_INFO("NEW_FILL for " << info._inst->exId() << " new position:" << info._position);
        info._order._filled += fill._qty;
        if(info._order.openedQty()==0)
        {
            info._order._status = OrderStatusFilled;
            LOG_INFO("Order " << info._inst->id() << ":" << info._order._order_id << " Filled");            
        }
        break;
    }
    default:
        throw std::runtime_error("Unexpected event type " + boost::lexical_cast<string>(event._base._type));
    }
    
}




    
}