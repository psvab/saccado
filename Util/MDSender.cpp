
#include "MDSender.h"
#include <set>
#include <boost/asio.hpp>
#include "Logger.h"
#include "Instref.h"
#include "Util.h"
#include "OrderOp.h"
#include "ComponentManager.h"


using namespace std;
using namespace boost;

namespace SC
{
  
    
void MDSender::load(const ptree& config)
{
    _instref = &Instref::instance();
    _ops.resize(0);
    _ops.reserve(1000000);
    if( config.count("minSleepUs") )
        _minSleepUs = config.get<int>("minSleepUs");
    if( config.count("maxSleepUs") )
        _minSleepUs = config.get<int>("maxSleepUs");
    std::string  data_file = config.get<string>("data");
    checked_ifstream f(data_file);    
    for(string line; getline(f, line); )
    {
        csvrow row(line,0);
        
        char inst[MAX_INST_ID];
        strncpy(inst, row[2].c_str(), MAX_INST_ID);
        Instrument* instPtr = &_instref->getByExchgIdOrThrow(row[2]);
        eBuySell side = strToSide(row[3]);
        eOrderOpCode op = strToOrderOpCode(row[4]);
        
        OrderOpExchgExt orderOp{{row.get<IntTime>(0), row.get<uint64_t>(1), 
                row.get<Price>(5),
                side, op, row.get<uint16_t>(6)}, inst, instPtr};
        _ops.push_back( orderOp );

        if( !_inst_info[instPtr->id()] )
            _inst_info[instPtr->id()] = make_unique<InstInfo>(instPtr->mdAddr());
    }
    
}

void MDSender::run()
{    
    IntTime data_prev = _ops.size() ? _ops[0]._time : 0;
    for(auto & op : _ops)
    {
        if(stopping() || ComponentManager::instance().stopping())
            break;
        
        IntTime dataGapUs = (data_prev - op._time)/1000;
        IntTime sleepForUs = min(_maxSleepUs, max(_minSleepUs, dataGapUs));      
        this_thread::sleep_for(chrono::microseconds(sleepForUs));
        
        auto& info = * _inst_info[ op._instPtr->id() ];  
        info._socket.sendto(reinterpret_cast<char*>(&op), sizeof(op), info._socket.addr());                                       
        ++_sent;
        data_prev = op._time;
    }
    LOG_INFO("All done, sent " <<  _sent << "  packets");
    ComponentManager::instance().shutdown();
}



    
    
    
}