#pragma once

#include <thread>
#include "Logger.h"
#include "Component.h"
#include "General.h"
#include "RingBuffer.h"
#include "MData.h"
#include "Event.h"


namespace SC
{
    
// Our main hub for passing data to trading strategy form various sources    
class EventQ : public ThreadRunningComponent
{    
public:
    EventQ() : _ringBuffer(65536)
    {}
    
    void registerListener(EventListener& listener) { _listener = &listener; }
    void unregisterListener() { _listener = &_dummyListener; }
    
    void addEvent(Event& e) { _ringBuffer.write(e); }        
    
protected:
    void stop() override;

    void run() override;
private:    
    struct DummyListener : EventListener
    {
        void handleEvent(const Event& event) {} 
    } _dummyListener;
    
    EventListener* _listener{&_dummyListener};
    RingBuffer<Event> _ringBuffer;
};




}