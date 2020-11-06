
#include <set>

#include "TradingLink.h"
#include "Event.h"
#include "Instref.h"
#include "ComponentManager.h"
#include "EventQ.h"

using namespace std;


namespace SC
{
  
void TradingLink::add_handled(Instrument& inst)
{
    _handledInst.insert(inst.id());    
}

void TradingLink::load(const ptree& config) 
{
    _exchg = config.get<string>("exchg");            
    if( config.count("insts") )
    {
        const auto& insts = config.get_child("insts");
        for(const auto & i : insts)
        {        
            std::string instid = i.second.data();
            Instrument& inst = Instref::instance().getByExchgIdOrThrow(instid);
            add_handled(inst);            
        }
    }else
    {
        for(const auto& instptr : Instref::instance().allInstref() )
            if( instptr->exchange() == _exchg )
                add_handled(*instptr);
    }
    _eventQ = &ComponentManager::instance().get_first<EventQ>();
}

void GoTLink::add_handled(Instrument& inst)
{
    TradingLink::add_handled(inst);
    strncpy(_infos[inst.id()]._reqNew._inst, inst.exId().c_str(), MAX_INST_LEN);
}


void GoTLink::load(const ptree& config)
{
    TradingLink::load(config);
    std::string addr = config.get<string>("addr");
    _addr = NetAddr(addr);    
}
void GoTLink::start()
{    
    try
    {
        _conn.connect(_addr);
    }
    catch(std::exception& e)
    {
        LOG_ERROR("Unable to connect to TCP exchange " + _addr.asStr());
        this_thread::sleep_for(500ms);
        exit(1);
    }
    TradingLink::start();
}


void GoTLink::run()
{
    TRY
    char buffer[4096];
    while(!stopping())
    {
        int read = _conn.read(buffer, sizeof(buffer));
        if(read == sizeof(FillExchg))
        {
            FillExchg& fillEx = *reinterpret_cast<FillExchg*>(buffer);
            fillEx._inst = Instref::instance().getByExchgIdOrThrow(fillEx._inst_ex).id();
            Event event;
            event._fill.set(fillEx);
            _eventQ->addEvent(event);
        }
    }
    FATAL_CATCH    
}



void GoTLink::sendOrderNew(Instrument* inst, Price price, int qty, eBuySell side ) 
{
    auto& reqNew = _infos[inst->id()]._reqNew;
    static int order_id = 0;
    ++order_id;
    reqNew._price = price;
    reqNew._order_id = order_id;
    reqNew._side = side;
    reqNew._qty = qty;
    _conn.send(reinterpret_cast<const char*>(&reqNew), sizeof(reqNew));    
}

    
    
}

