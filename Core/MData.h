#pragma once

#include <cassert>
#include "General.h"

namespace SC
{

// we can store the values together, let us store touch for now only
struct BidAskVals
{
    Price _bid{0};
    Price _ask{0};
    int _bid_qty{0};
    int  _ask_qty{0};    
    
    bool operator == (const BidAskVals& vals) const
    {
        return memcmp(this, &vals, sizeof(vals)) == 0;
    }
    bool operator != (const BidAskVals& vals) const
    {
        return !(*this == vals);
    }
    bool valid() const { return _bid_qty && _ask_qty && _bid<_ask; }
};
    
/// Internal market data image (BBO)
struct MData
{
    InstrumentId _instId{0}; 
    IntTime  _time_exchg{0}; // time data was sent on the exchange
    IntTime  _time{0};  // time md received
    BidAskVals _quote; // the price information
};  



        
}

inline std::ostream& operator << (std::ostream& out, const SC::BidAskVals& md)
{
    out << md._bid_qty << " " << md._bid << " "<< md._ask << " " << md._ask_qty;
    return out;
}

inline std::ostream& operator << (std::ostream& out, const SC::MData& md)
{
    out << md._instId << " " << md._time << " "<< md._time_exchg << " " << md._quote;
    return out;
}

