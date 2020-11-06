#pragma once
#include "General.h"


namespace SC
{

enum eOrderStatus {
    OrderStateDefault,
    OrderStatusActive,
    OrderStatusCancelled,
    OrderStatusFilled,
    OrderStatusCount
};
    
// Order    
struct Order
{
    int openedQty() const { return _qty - _filled; }
    
    void set_new(Price price, int qty, eBuySell side)
    {
        _order_id++;
        _qty=qty;
        _price =price;
        _side=side;
        _filled=0;
        _status = OrderStatusActive;        
    }

    uint64_t _order_id{1};
    int    _iid{0};
    eBuySell _side{SideCount};
    int    _qty{0};
    int    _filled{ 0 };
    double _price{ 0 };
    
    eOrderStatus _status{OrderStatusCount};
};


}

inline std::ostream& operator << ( std::ostream& out, const SC::Order& o)
{
    out << o._order_id << " "<< o._iid << " " << o._side << " " << o._price << " " << o._qty << " " << o._filled<< " " << o._status;
    return out;
}
