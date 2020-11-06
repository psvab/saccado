


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
#include "Strategy.h"

using namespace std;
using namespace SC;




TEST(Strategy, strat_test_limits)
{
    ComponentManager& mgr = ComponentManager::instance();
    mgr.load_from_json("tests/strattest.json");
    Strategy1& strat = mgr.get_by_name<Strategy1>("strat1");
    
    IntTime now = chrono::system_clock::now().time_since_epoch().count();
    MData md1{1,now, now+1,   100,200,1,1};
    Event event;
    EventMData& emd = event._md;        
    emd.set(md1);
    strat.handleEvent(event);

    MData& md = emd._md;
    for(int i=0; i<10; ++i)
    {
        md._quote._bid++;   
        strat.handleEvent(event);
    }
    ASSERT_EQ(strat.ordersSent(), 1); // limit is 3
    
    md._time += 1.1e9;
    md._time_exchg += 1e9; // do not move that one enough
    for(int i=0; i<10; ++i)
    {
        md._quote._bid++;   
        strat.handleEvent(event);
    }
    ASSERT_EQ(strat.ordersSent(), 1); 
    
    
    // test fill limit
    Fill fill{md._time, 1, 1, 100, Sell, 1};    
    Event event2;
    EventFill& fillevent = event2._fill;
    fillevent.set(fill);
    for(int i=0; i<4; ++i)
    {
        strat.handleEvent(event2);
    }
    
    md._time += 2.1e9;
    md._time_exchg += 2e9; // do not move that one enough
    for(int i=0; i<10; ++i)
    {
        md._quote._bid++;   
        strat.handleEvent(event);
    }
    ASSERT_EQ(strat.ordersSent(), 1);  // no more trading
    
    fillevent._fill._side = Buy;
    for(int i=0; i<1; ++i)
    {
        strat.handleEvent(event2);
    }
    
    md._time += 2.1e9;
    md._time_exchg += 2e9; // do not move that one enough
    for(int i=0; i<10; ++i)
    {
        md._quote._bid++;   
        strat.handleEvent(event);
    }
    ASSERT_EQ(strat.ordersSent(), 2);  //  more could trade

    mgr.tear_down();
    
}

