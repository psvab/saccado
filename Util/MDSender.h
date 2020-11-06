#pragma once

#include <thread>
#include "General.h"
#include "Logger.h"
#include "OrderOp.h"
#include "Component.h"
#include "Network.h"

namespace SC
{
    
/// Class to load and send multicast     
class MDSender : public ThreadRunningComponent
{
public:
    // how may messages have been sent
    size_t sent() const { return _sent; }
protected:
    
    void load(const ptree& config) override;
        
private:
    
    struct OrderOpExchgExt : OrderOpExchg
    {
        OrderOpExchgExt() =default;
        OrderOpExchgExt(const OrderOpBase& op, const char *s, Instrument* inst_) :
            OrderOpExchg(op,s), _instPtr(inst_) 
        {}
        Instrument* _instPtr{nullptr};
    };
    
    struct InstInfo
    {
        InstInfo() = default;
        InstInfo(const std::string& addr) : _socket(addr)
        {};        
        McastSendSocket _socket;        
    };
    
    
    void run();
    
    Instref* _instref{nullptr}; // local pointer to instref
    size_t _sent{0};
    std::vector<OrderOpExchgExt> _ops;
    std::array<std::unique_ptr<InstInfo>, MAX_INST_ID> _inst_info;
    IntTime _minSleepUs{1000};
    IntTime _maxSleepUs{1000*1000};
    
};


}


