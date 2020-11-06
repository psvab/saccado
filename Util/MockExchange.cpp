


#include "MockExchange.h"

using namespace std;


namespace SC
{
  
    
void MockExchange::load(const ptree& config) 
{
    string addr = config.get<string>("addr");            
    _addr = NetAddr(addr);    
    _server.setAddr(_addr);
    _server.server_bind();    
}

 


void MockExchange::run() 
{
    TRY
    while(!stopping())
    {        
        _server.accept_connection(_conn);
        LOG_INFO("Connected");
        while(!stopping())
        {
            char buffer[4096];
            int count = _conn.read(buffer, sizeof(buffer));
            if( count == sizeof(NewOrderRequest))
            {
                // echo with a fill
                NewOrderRequest& req = *reinterpret_cast<NewOrderRequest*>(buffer);
                FillExchg fill{chrono::system_clock::now().time_since_epoch().count(),
                        0, req._order_id, req._price, req._side, req._qty };            
                strncpy(fill._inst_ex, req._inst, sizeof(fill._inst_ex));
                LOG_INFO("Order "<< req._order_id << " received, responding with a fill.");
                _conn.send(reinterpret_cast<const char*>(&fill), sizeof(fill));
            }
            if(count==sizeof(EndCommsReq))
            {
                LOG_INFO("EndCommsReq received");
                return;
            }
        }
    }
    FATAL_CATCH
}


    
    


}