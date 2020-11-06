#pragma once

#include "Logger.h"
#include "General.h"
#include "MData.h"
#include "Order.h"

namespace SC
{
    
    
enum eEventType  {
    EventTypeReserved,
    EventTypeMData,
    EventTypeFill,
    EventTypeOrderUpdate,
    EventTypeFnc,
    EventTypeCount
};

    
/// Simple fill class
struct Fill
{
    IntTime      _time;       // time fill received
    InstrumentId _inst{0};    //inst id
    uint64_t    _order_id{0}; // order id 
    Price       _price{0};
    eBuySell    _side{SideCount}; // side of the order that was filled
    int         _qty{0};      // fill qty
};

struct OrderUpdate
{
    IntTime      _time;       // time order update received
    InstrumentId _inst{0};    //inst id
    uint64_t    _order_id{0}; // order id 
    eOrderStatus _status{OrderStatusCount};      
};


// Fill coming from the exchange; to simplify conversions, we assume it is 
// containing also our format.. very handy, thank you Go exchange
struct FillExchg : Fill
{
    char        _inst_ex[MAX_INST_LEN];    
};

// New request to be sent to the exchange
struct NewOrderRequest
{
    uint64_t    _order_id{0}; // order id 
    char        _inst[MAX_INST_LEN];
    Price _price{0};
    int   _qty{0};
    eBuySell _side{SideCount}; 
};

// terminator for connection with mock exchange
struct EndCommsReq
{
    int endcomms;
};


struct EventBase
{
    eEventType _type;
};

struct EventMData : EventBase
{
    void set(const MData& md)     
    {
        _type = EventTypeMData;
        _md = md;
    }
    
    MData _md;
};
    
struct EventFill : EventBase
{
    void set(const Fill& fill)     
    {
        _type = EventTypeFill;
        _fill = fill;
    }
    Fill _fill;
};

struct EventOrderUpdate : EventBase
{
    void set(const OrderUpdate& ou)     
    {
        _type = EventTypeOrderUpdate;
        _update = ou;
    }
    OrderUpdate _update;
};


/// Event type for our comms ring buffer
struct Event
{
    Event(){}
    union
    {
        EventBase _base;
        EventMData _md;
        EventFill _fill;
        EventOrderUpdate _order_update;
    };        
};


struct EventListener
{
    virtual ~EventListener() {}
    virtual void handleEvent(const Event& event) = 0;
};




}