
#pragma once

#include <gtest/gtest.h>


#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>

#include "General.h"
#include "Network.h"

using namespace std;
using namespace SC;


TEST(General, buysell)
{
    ASSERT_EQ( BuySellTraits<Buy>::better(1,3.0), false );
    ASSERT_EQ( BuySellTraits<Buy>::worse(1,3.0), true );
    ASSERT_EQ( BuySellTraits<Buy>::worse(1,1), false );
    ASSERT_EQ( BuySellTraits<Buy>::better(1,1), false );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Buy>::idx), 0 );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Buy>::inc), -1 );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Buy>::side), Buy );
    
    ASSERT_EQ( BuySellTraits<Sell>::better(1,3.0), true );
    ASSERT_EQ( BuySellTraits<Sell>::worse(1,3.0), false );
    ASSERT_EQ( BuySellTraits<Sell>::worse(1,1), false );
    ASSERT_EQ( BuySellTraits<Sell>::better(1,1), false );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Sell>::idx), 1 );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Sell>::inc), 1 );
    ASSERT_EQ( static_cast<int>(BuySellTraits<Sell>::side), Sell );
}
    

TEST(Network, Addr)
{
    {
        NetAddr addr("244.1.1.1:3001");
        ASSERT_EQ(addr.ip(), "244.1.1.1");
        ASSERT_EQ(addr.port(), 3001);
        ASSERT_EQ(addr.iface(), "");
        ASSERT_EQ(addr.asStr(), "244.1.1.1:3001");
        ASSERT_THROW(NetAddr("hello"), std::exception);
    }
    {
        NetAddr addr("244.1.1.1:3001:eth0");
        ASSERT_EQ(addr.ip(), "244.1.1.1");
        ASSERT_EQ(addr.port(), 3001);
        ASSERT_EQ(addr.iface(), "eth0");
        ASSERT_EQ(addr.asStr(), "244.1.1.1:3001:eth0");
    }    
}
