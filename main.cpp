#pragma once

#include <fstream>

#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

#include "./include/converterJSON.h"
#include "./include/invertedIndex.h"
#include "./include/searchServer.h"
#include "./include/tests.h"
#include "./include/additional.h"

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    generate_test_state(0);
    auto converter = new ConverterJSON;
    auto idx = new InvertedIndex();
    idx->UpdateDocumentBase(converter->GetTextDocuments());
    auto searchServer = new SearchServer(*idx);
    converter -> putAnswers(transform(searchServer->Search(converter->GetRequests())));
    return RUN_ALL_TESTS();
}