
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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "TradingLink.h"

using namespace std;
using namespace SC;
namespace fs = std::filesystem;


TEST(ComponentManager, load_and_find_comps)
{
    struct NotIn : Component
    {
    };
    ComponentManager& mgr = ComponentManager::instance();
    
    boost::property_tree::ptree pt;
    std::stringstream conf("[ {\"class\":\"Blah\" } ]");
    boost::property_tree::read_json(conf, pt);
    EXPECT_EXIT(mgr.load(pt), testing::ExitedWithCode(1), "");
       
    mgr.load_from_json("tests/cmpmgrtest.json");
    ASSERT_EQ( mgr.component_exists("Blah"), false);
    ASSERT_EQ( mgr.component_exists("tlink1"), true);
    ASSERT_THROW( mgr.get_first<NotIn>(), std::exception );
    ASSERT_EQ( &mgr.get_first<TradingLink>(), &mgr.get_by_name<TradingLink>("tlink1") );
    
    ASSERT_EQ( mgr.get_by_name<TradingLink>("tlink1").name() , "tlink1");
    ASSERT_EQ( &mgr.get_by_name<TradingLink>("tlink1") ,  &mgr.get_by_name("tlink1") );
    mgr.tear_down();
}

