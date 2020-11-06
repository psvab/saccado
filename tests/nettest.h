
#pragma once

#include <gtest/gtest.h>

#include "ComponentManager.h"

#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <thread>
#include <chrono>
#include "Network.h"

using namespace std;
using namespace SC;



TEST(NetTest, mcast)
{
    TRY
    NetAddr group("224.1.1.1:5002");
    std::mutex _mutex;
    int send_count = 10;
    const string msg = "Useful data";            
    
    auto sender_fnc = [&]() {
        TRY
        McastSendSocket sock(group);           
        this_thread::sleep_for(200ms); // enable the listener to start
        for (int i=0; i<send_count; ++i) 
        {            
            sock.sendtoOrThrow(msg.c_str(), msg.size(), group);
            this_thread::sleep_for(50ms);            
        }
        FATAL_CATCH
    };
    auto receiver_fnc = [&]() {
        TRY
        McastRecvSocket sock(group);    
        sock.join(group);
        int recv_count = 0;
        auto start = chrono::system_clock::now();
        for (; recv_count<send_count && chrono::system_clock::now()-start < 2s;) 
        {
            char buffer[65536];
            int len = sock.recvfrom(buffer, sizeof(buffer));
            if(len>0)
            {
               ASSERT_EQ(string(buffer, buffer+len), msg) ;
               ++recv_count;               
            } 
        }
        ASSERT_EQ(recv_count, send_count);
        FATAL_CATCH
    };
    
    thread sender(sender_fnc);
    receiver_fnc();
    sender.join();
    
    FATAL_CATCH
}



TEST(NetTest, tcp)
{
    TRY
    NetAddr addr("127.0.0.1:8080");
    const string msg = "Useful data";            
    
     auto sender_fnc = [&]() {
        TRY
        this_thread::sleep_for(200ms); // enable the listener to start        
        TCPSocket socket(addr);
        socket.connect();
        socket.send(msg.c_str(), msg.size());     
        char buffer[1024];
        int count = socket.read(buffer, sizeof(buffer));        
        ASSERT_EQ(string(buffer, buffer+count), msg);
        FATAL_CATCH
    };
    auto receiver_fnc = [&]() {
        TRY
        TCPSocket sock(addr);    
        sock.server_bind();
        Socket conn;
        sock.accept_connection(conn);
        char buffer[1024];
        int count = conn.read(buffer, sizeof(buffer));        
        ASSERT_EQ(string(buffer, buffer+count), msg);
        conn.send(msg.data(), msg.size());
        FATAL_CATCH
    };
    
    thread sender(sender_fnc);
    receiver_fnc();
    sender.join();
    FATAL_CATCH
}