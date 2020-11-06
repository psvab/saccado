
#pragma once

#include <gtest/gtest.h>

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>
#include "Instref.h"
#include "Book.h"


using namespace std;
using namespace SC;


/*
struct OrderOpBase    
{
    IntTime  _time{0};
    uint64_t _order_id{0};
    Price _price{0};    
    eBuySell _side{SideCount};
    eOrderOpCode  _op{OpOrderCount};
    uint16_t _qty{0};       
};*/

TEST(Book, build)
{
    BidAskVals zeroq;
    Book book(1);
    ASSERT_EQ(book.changed(), false);
    ASSERT_TRUE( book.latest()._quote == zeroq );
    
    // test bid 
    book.apply_op({1,1,100,Buy,OpOrderNew, 3});
    ASSERT_EQ(book.changed(), true);
    
    book.apply_op({1,2,101,Buy,OpOrderNew, 5, 1000});    
    ASSERT_EQ(book.changed(), true);
    
    book.apply_op({1,3,99,Buy,OpOrderNew, 7});
    ASSERT_EQ(book.changed(), false);
    
    ASSERT_EQ(book.latest()._quote._bid, 101);
    ASSERT_EQ(book.latest()._quote._bid_qty, 5);
    ASSERT_EQ(book.latest()._time_exchg, 1);
    ASSERT_EQ(book.latest()._time, 1000);
    
    
    book.apply_op({2, 1, 100,Buy, OpOrderCancel, 3});
    ASSERT_EQ(book.changed(), false);
    
    book.apply_op({2, 2, 100,Buy, OpOrderCancel, 5});
    ASSERT_EQ(book.changed(), true);
    
    ASSERT_EQ(book.latest()._quote._bid, 99);
    ASSERT_EQ(book.latest()._quote._bid_qty, 7);
    
    // now test offer
    
    book.apply_op({3,10,100,Sell,OpOrderNew, 3});
    ASSERT_EQ(book.changed(), true);
    
    book.apply_op({3,11,101,Sell,OpOrderNew, 5});
    ASSERT_EQ(book.changed(), false);
    
    book.apply_op({3,12,99,Sell,OpOrderNew, 7});
    ASSERT_EQ(book.changed(), true);
        
    ASSERT_EQ(book.latest()._quote._ask, 99);
    ASSERT_EQ(book.latest()._quote._ask_qty, 7);
    
    book.apply_op({4, 12, 99, Sell, OpOrderCancel, 7});
    ASSERT_EQ(book.changed(), true);
    
    ASSERT_EQ(book.latest()._quote._ask, 100);
    ASSERT_EQ(book.latest()._quote._ask_qty, 3);
    
    
    book.apply_op({4, 10, 100, SideCount, OpOrderTrade, 2});
    ASSERT_EQ(book.changed(), true);
    ASSERT_EQ(book.latest()._quote._ask, 100);
    ASSERT_EQ(book.latest()._quote._ask_qty, 1);
    
    book.apply_op({4, 10, 100, SideCount, OpOrderTrade, 1});
    ASSERT_EQ(book.changed(), true);
    ASSERT_EQ(book.latest()._quote._ask, 101);
    ASSERT_EQ(book.latest()._quote._ask_qty, 5);
    
}

