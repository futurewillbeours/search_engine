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
#include "./include/additional.h"

//todo: исправить индексирование по многопоточности

std::vector<std::vector<std::pair<int, float>>> transform(std::vector<std::vector<RelativeIndex>> search) {
    std::vector<std::vector<std::pair<int, float>>> result;
    for(auto& s1:search) {
        std::vector<std::pair<int, float>> vec;
        for (auto& s2:s1) vec.push_back(std::pair<int, float>{(int)s2.doc_id, s2.rank});
        result.push_back(vec);
    }
    return result;
}



int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    std::srand(std::time(nullptr));
    auto generator = new Generator;
    generator -> generate_test_state();
    auto converter = new ConverterJSON;
    auto invertedIndex = new InvertedIndex();
    invertedIndex->UpdateDocumentBase(converter->GetTextDocuments());
    auto searchServer = new SearchServer(*invertedIndex);

    print_transform(transform(searchServer->Search(converter->GetRequests())));

    converter -> putAnswers(transform(searchServer->Search(converter->GetRequests())));
    int o; std::cin >> o;
    return RUN_ALL_TESTS();
}