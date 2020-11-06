    
// This file is to hold our trading connectivity - on both thread sides

#pragma once
#include <set>
#include "General.h"
#include "Component.h"
#include "Network.h"
#include "Event.h"


namespace SC
{
class Instrument;
class EventQ;

// A generic trading link component    
class TradingLink : public ThreadRunningComponent
{
public:
    
    virtual void sendOrderNew(Instrument* inst, Price price, int qty, eBuySell side ) 
    {
        throw std::invalid_argument("not implemented send order handler");
    }
    
    virtual bool handles(InstrumentId iid) { return _handledInst.find(iid) != _handledInst.end(); }
    
protected:    
    virtual void add_handled(Instrument& inst);
    void load(const ptree& config) override;
    
    std::string _exchg; ///< Exchange name
    std::set<InstrumentId> _handledInst;
    EventQ *_eventQ;
};
    

/// Go trading link which is our GO exchange protocol trading link
class GoTLink : public TradingLink
{
public:
    void sendOrderNew(Instrument* inst, Price price, int qty, eBuySell side ) override;
    void load(const ptree& config) override;
protected:
    void add_handled(Instrument& inst) override;
    void run();  
    void start() override;
    
private:
    
    NetAddr   _addr; // address for requests/responses
    TCPSocket _conn;
        
    struct InstInfo
    {
        NewOrderRequest _reqNew;
    };
    
    InstInfo _infos[MAX_INST_ID];
    
};


class Strategy;
// Mock tlink for testing
struct MockTLink : TradingLink
{
    void sendOrderNew(Instrument* inst, Price price, int qty, eBuySell side ) override 
    {
    }    
    void run() {}
    
};
    
    
}

