
#pragma once



#include "General.h"
#include "Component.h"
#include "Instref.h"

namespace SC
{
   
    
enum eOrderOpCode {
    OpOrderNew,
    OpOrderCancel,
    OpOrderTrade,
    OpOrderCount
};  

// Shared for exchange and us
struct OrderOpBase    
{
    IntTime  _time{0};  // time  s delivered (exchange time)
    uint64_t _order_id{0};
    Price _price{0};    
    eBuySell _side{SideCount};
    eOrderOpCode  _op{OpOrderCount};
    uint16_t _qty{0};       
};

// Our side order operation
struct OrderOp : OrderOpBase  
{    
    IntTime _time_recv{0};
};

// Mock exchange side data
struct OrderOpExchg : OrderOpBase   
{
    OrderOpExchg() = default;
    OrderOpExchg(const OrderOpBase& op, const char *s) : OrderOpBase(op) {
        strncpy(_inst,s, MAX_INST_LEN);
    }
    char _inst[MAX_INST_LEN];
};


inline eOrderOpCode strToOrderOpCode(const std::string& op_code)
{
    if(op_code=="NEW") return OpOrderNew;
    if(op_code=="CANCEL") return OpOrderCancel;
    if(op_code=="TRADE") return OpOrderTrade;
    throw std::runtime_error("Unknown order opcode" + op_code);
}

    
    
    
}