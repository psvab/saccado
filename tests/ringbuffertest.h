#pragma once
#include <gtest/gtest.h>

#include "RingBuffer.h"

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

using namespace std;
using namespace SC;



TEST(DataQ, running)
{
	cout << "dataq test" << endl;
	int stop = 0;
	atomic<int> value;
        value.store(1);
	vector<int> vals;
	RingBuffer<int> q(2);
        
        q.write(1);
        q.write(2);
        ASSERT_EQ(q.size(),2);
        int val;
        q.read(val);
        ASSERT_EQ(q.size(),1);
        ASSERT_EQ(val,1);
        q.read(val);
        ASSERT_EQ(val,2);
        ASSERT_EQ(q.size(),0);
	auto prodfnc = [&]() {	
		while (!stop){
			int val = value.fetch_add(1, memory_order_release);
			q.write(val);
		}
		q.write(-1);
		cout << "end write\n";
	};
	vector<thread> thr;
	thr.emplace_back(prodfnc);
	thr.emplace_back(prodfnc);
	thr.emplace_back(prodfnc);
	int wsize = thr.size();
	thr.emplace_back([&]() {
		int wend = 0,  prev = 0;
		while (wend<wsize) {			
			int val;
			q.read(val);
			vals.push_back(val);		
			if (val < 0) wend++;			
			prev = val;
		}
		cout << "end read\n";
		});

	this_thread::sleep_for(0.5s);
	stop = 1;
	for (auto& t : thr)
	{
		cout << " joining\n";
		t.join();
	}
	cout << vals.size() << endl << " test end"<< endl;
        EXPECT_GT( vals.size(), 0);
}

