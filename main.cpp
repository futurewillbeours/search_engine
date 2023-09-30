#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <map>
#include <sstream>

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

#include "./include/generator.h"
#include "./include/converterJSON.h"
#include "./include/invertedIndex.h"
#include "./include/tests.h"
#include "./include/searchServer.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::srand(std::time(nullptr));
    auto generator = new Generator;
    generator -> generate_state();
    auto converter = new ConverterJSON;
    auto invertedIndex = new InvertedIndex();
    invertedIndex->UpdateDocumentBase(converter->GetTextDocuments());
    invertedIndex->print_freq_dictionary();

    int o; std::cin >> o;
    return RUN_ALL_TESTS();
}