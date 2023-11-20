#pragma once

#include <fstream>

#include "nlohmann/json.hpp"

#include "./../include/converterJSON.h"
#include "./../include/invertedIndex.h"
#include "./../include/searchServer.h"
#include "./../include/additional.h"

int main(int argc, char* argv[]) {
    auto converter = new ConverterJSON;
    auto idx = new InvertedIndex();
    try {
        idx->updateDocumentBase(converter->getTextDocuments());
        auto searchServer = new SearchServer(*idx);
        converter -> putAnswers(transform(searchServer->search(converter->getRequests())));
        std::cout << "Program finished, answers.json file created!\n";
    } catch (const std::runtime_error& x) {
        std::cerr << "Caught runtime_error exception: " << x.what() << std::endl;
        std::cout << "Program finished with runtime error!\n";
    } catch (const nlohmann::json_abi_v3_11_2::detail::parse_error& x) {
        std::cerr << "Caught nlohmann_json exception: " << x.what() << std::endl;
        std::cout << "Program finished with nlohmann_json file error!\n";
    }
}