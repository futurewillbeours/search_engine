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

std::vector<std::vector<std::pair<int, float>>> transform(std::vector<std::vector<RelativeIndex>> search) {// функция преобразования аргументов
    std::vector<std::vector<std::pair<int, float>>> result;
    for(auto& s1:search) {
        std::vector<std::pair<int, float>> vec;
        for (auto& s2:s1) vec.push_back(std::pair<int, float>{(int)s2.doc_id, s2.rank});
        result.push_back(vec);
    }
    return result;
}

void print_transform(std::vector<std::vector<std::pair<int, float>>> result) {
    for(int i = 0; i < result.size(); i++) {
        std::cout << i << ", ";
        for (int j = 0; j < result[i].size(); j++) {
            std::cout << j << ": doc_id: " << result[i][j].first << ", rank: " << result[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
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

    //print_transform(transform(searchServer->Search(converter->GetRequests())));

    converter -> putAnswers(transform(searchServer->Search(converter->GetRequests())));
    int o; std::cin >> o;
    return RUN_ALL_TESTS();
}