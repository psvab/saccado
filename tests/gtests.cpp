
#include <gtest/gtest.h>

#include "nettest.h"
#include "booktest.h"
#include "utilstest.h"
#include "instreftest.h"
#include "ringbuffertest.h"
#include "componentmanagertest.h"
#include "mdsendertest.h"
#include "mockexchgtest.h"
#include "strattest.h"
 


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS();
}