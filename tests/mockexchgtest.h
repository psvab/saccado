


#pragma once

#include <gtest/gtest.h>

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "ComponentManager.h"
#include "Instref.h"
#include "MockExchange.h"

using namespace std;
using namespace SC;


TEST(MockExchange, fill_test)
{
    ComponentManager& mgr = ComponentManager::instance();
    mgr.load_from_json("tests/mockex.json");
    mgr.start_all();    
    MockExchange& exch = mgr.get_by_name<MockExchange>("Go1");
    LOG_INFO("started)")
    TCPSocket sock(exch.addr());
    sock.connect();
    NewOrderRequest req;
    strcpy(req._inst, "INST");
    req._price = 100;
        
    for(int i=0; i<5; ++i)
    {
        req._order_id++;
        req._price++;
        req._qty = 1;
        req._side = Buy;  
        sock.send(reinterpret_cast<char*>(&req), sizeof(req));
        
        char buffer[4096];
        int count = sock.read(buffer, sizeof(buffer));
        ASSERT_EQ(count, sizeof(FillExchg));
        FillExchg& fill = *reinterpret_cast<FillExchg*>(buffer);
        ASSERT_EQ(fill._order_id, req._order_id);
        ASSERT_EQ(fill._price, req._price);
        ASSERT_EQ(fill._side, req._side);
        ASSERT_EQ(fill._qty, req._qty);        
    }
    EndCommsReq reqe;
    sock.send(reinterpret_cast<char*>(&reqe), sizeof(reqe));
    LOG_INFO("done")
    mgr.tear_down();
    
}

