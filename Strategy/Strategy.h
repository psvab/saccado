#pragma once

#include "EventQ.h"
#include "TradingLink.h"
#include "Order.h"

namespace SC
{

// Base for all strategies    
class Strategy : public EventListener, public Component
{
public:
};
    

// My one chosen strategy
class Strategy1 : public Strategy
{
public:
    
    //Return the total number of orders - this is for testing and only because there is no time to create a proper trading link mock. TODO: improve mock
    int ordersSent() const { return _orders_sent; }
    
    void handleEvent(const Event& event) override;
protected:
    void load(const ptree& config) override;    
    void stop() override;    
    
private:
    struct InstInfo
    {
        Instrument * _inst{nullptr};
        MData _md;
        
        int _sent_qty {0};
        int _position {0};
        int _last_time{0};
        
        int _max_position{0};
        int _max_orders{0}; 
        Order  _order; // we only ever have one active order
                
        TradingLink* _tlink{nullptr};
    };    
    
    // New recalculate point - review situation and send orders
    void recalculate(InstInfo& info, const MData& newMD);
    // Try send order - do risk checks to
    // TODO: more risk checks
    void tryOrder(InstInfo& info, Price price, int qty, eBuySell side);
    
    
    IntTime _time{0};
    std::array<InstInfo, MAX_INST_ID> _instInfo;    
    
    int _max_orders{0};    // maximal number of order in 2^30 ns  (1.07s)
    int _max_position{0};
    int _orders_sent{0};
};

}



