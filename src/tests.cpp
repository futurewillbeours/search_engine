#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

#include "./../include/tests.h"
#include "./../include/additional.h"

int main (int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    generateTestState(0);
    RUN_ALL_TESTS();
    deleteTestState();
}