#pragma once

#include "General.h"
#include "Logger.h"
#include "Component.h"
#include "Network.h"
#include "Event.h"

namespace SC
{
   
    
// Mock exchange    
class MockExchange : public ThreadRunningComponent
{
public:
    const NetAddr& addr() { return _addr; }
    
protected:
    void load(const ptree& config) override;
    void run() override;         
private:
    NetAddr _addr;
    TCPSocket _server;
    Socket _conn; // we can only connect one trader now
};
    
    
}


