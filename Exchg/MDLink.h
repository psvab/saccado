
// This file is to hold our trading connectivity - on both thread sides

#pragma once

#include <array>
#include <thread>
#include "General.h"
#include "Component.h"
#include "Book.h"
#include "Network.h"


namespace SC
{

class  EventQ;
class Intrument;

    
// A generic market data listening and book building component    
class MDLink : public ThreadRunningComponent
{
public:
    
private:
};
    

/// Go market data link which is our GO exchange protocol MD link
/// This MDLink subscibes to only one instrument mainly to demonstrate we can have more threads. 
//  TODO: Enable subscribing to multiple instruments In real wolkd it would be better to subscribe to more on one listener thread
class GoMDLink : public MDLink
{
public:
    void load(const ptree& config) override;
    
    void start() override;
    void stop() override;
    
private:
    void run();
    // process one order update
    void onOrderUpdate(const OrderOp& op);
    
    std::string      _instId; // store exchange id to check we are receiving only our instrument
    Instrument      *_inst{nullptr};
    unique_ptr<Book> _book;          // book of our instrument
    EventQ* _eventQ;     
    IntTime         _lastTimeRecv{0};
     
    McastRecvSocket _socket; // listening socket    
};

    
    
}

