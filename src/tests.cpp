#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

// #include "./../include/converterJSON.h"
// #include "./../include/invertedIndex.h"
// #include "./../include/searchServer.h"
// #include "./../include/additional.h"
#include "./../include/tests.h"

int main (int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}