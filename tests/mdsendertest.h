
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
#include "MDSender.h"


using namespace std;
using namespace SC;
namespace fs = std::filesystem;



TEST(MDSenderTest, replay)
{
    ComponentManager& mgr = ComponentManager::instance();
    mgr.load_from_json("tests/mdsend.json");
    mgr.run(false);    
    MDSender& sender = mgr.get_by_name<MDSender>("sender1");
    ASSERT_TRUE( sender.sent() >0 );
    
    mgr.tear_down();
    
}

