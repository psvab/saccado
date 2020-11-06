#include "EventQ.h"


namespace SC
{
 
void EventQ::stop() 
{
    unregisterListener();
    Event event;
    addEvent(event);
    ThreadRunningComponent::stop();
}    
    
void EventQ::run() 
{
    Event event;
    while(!stopping())
    {
        _ringBuffer.read(event);
        _listener->handleEvent(event);
    }    
}




}