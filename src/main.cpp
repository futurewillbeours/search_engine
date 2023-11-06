#pragma once

#include <fstream>

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

#include "./../include/converterJSON.h"
#include "./../include/invertedIndex.h"
#include "./../include/searchServer.h"
#include "./../include/tests.h"
#include "./../include/additional.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    generateTestState(0);
    auto converter = new ConverterJSON;
    auto idx = new InvertedIndex();
    try {
        idx->updateDocumentBase(converter->getTextDocuments());
        auto searchServer = new SearchServer(*idx);
        converter -> putAnswers(transform(searchServer->search(converter->getRequests())));
        std::cout << "Program finished, answers.json file created!\n";
    } catch (const std::runtime_error& x) {std::cerr << "Caught runtime_error exception: " << x.what() << std::endl;}
    //return RUN_ALL_TESTS();
}