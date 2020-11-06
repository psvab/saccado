
#include "MDLink.h"
#include "EventQ.h"
#include "ComponentManager.h"

using namespace std;

namespace SC
{
  
    
   
void GoMDLink::load(const ptree& config)
{    
    _instId = config.get<string>("inst");
    _inst = &Instref::instance().getByExchgIdOrThrow(_instId);
    _socket.setAddr( NetAddr(_inst->mdAddr()) );    
    _book = make_unique<Book>(_inst->id());
    _eventQ = &ComponentManager::instance().get_first<EventQ>();
}

void GoMDLink::start()
{
    _socket.join( _socket.addr() );
    ThreadRunningComponent::start();    
}

void GoMDLink::stop()
{
    _socket.leave( _socket.addr() );
    ThreadRunningComponent::stop();
}


inline void GoMDLink::onOrderUpdate(const OrderOp& op)
{
    _book->apply_op( op );
    if(_book->changed() )
    {
        Event event;
        event._md.set( _book->latest() );
        LOG_DEBUG(event._md._md);
        _eventQ->addEvent( event );
    }
}

void GoMDLink::run()
{
    char buffer[4096];
    while(!stopping())
    {
        int recv = _socket.recvfrom(buffer, sizeof(buffer));
        _lastTimeRecv = chrono::high_resolution_clock::now().time_since_epoch().count();
        if(recv>0)
        {
            if(recv>=sizeof(OrderOpExchg)) // we only expect market data here
            {
                OrderOpExchg& op = *reinterpret_cast<OrderOpExchg*>(buffer);
                if( strncmp(op._inst, _instId.data(), _instId.size()) )
                {
                    // not our instrumemnt
                    LOG_DEBUG("Unexpected instrument received : " << std::string(op._inst));
                }else
                {
                    OrderOp& opext = *reinterpret_cast<OrderOp*>(buffer);
                    opext._time_recv = _lastTimeRecv;                    
                    onOrderUpdate(opext);
                }
            }else
                LOG_ERROR("Unexpected data received with size " << recv);
        }
    }
}

    
    
}

