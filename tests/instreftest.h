
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
#include "Instref.h"

using namespace std;
using namespace SC;
namespace fs = std::filesystem;



TEST(InstrefTest, load_and_find)
{
    ComponentManager& mgr = ComponentManager::instance();
    mgr.load_from_json("tests/cmpmgrtest.json");
    // Component manager initialises instref after logger
    auto& I = Instref::instance();
    ASSERT_TRUE(  I.getById(1) !=nullptr );
    ASSERT_THROW( I.getByIdOrThrow(3), std::exception );
    ASSERT_EQ( I.getById(1)->exchange(), "Go1" );
    ASSERT_EQ( I.getById(1),  I.getByExchgId("HSIF9") );
    mgr.tear_down();
}

