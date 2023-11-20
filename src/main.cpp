#pragma once

#include <fstream>
#include <memory>

#include "nlohmann/json.hpp"

#include "./../include/converterJSON.h"
#include "./../include/invertedIndex.h"
#include "./../include/searchServer.h"
#include "./../include/additional.h"

int main(int argc, char* argv[]) {
    generateTestState(0);
    std::unique_ptr<ConverterJSON> converterPtr = std::make_unique<ConverterJSON>();
    std::unique_ptr<InvertedIndex> idxPtr = std::make_unique<InvertedIndex>();
    try {
        idxPtr.get() -> updateDocumentBase(converterPtr.get() -> getTextDocuments());
        std::unique_ptr<SearchServer> searchServerPtr = std::make_unique<SearchServer>(*(idxPtr.get()));
        converterPtr.get() -> putAnswers(transform(searchServerPtr.get() -> search(converterPtr.get() -> getRequests())));
        std::cout << "Program finished, answers.json file created!\n";
    } catch (const std::runtime_error& x) {
        std::cerr << "Caught runtime_error exception: " << x.what() << std::endl;
        std::cout << "Program finished with runtime error!\n";
    } catch (const nlohmann::json_abi_v3_11_2::detail::parse_error& x) {
        std::cerr << "Caught nlohmann_json exception: " << x.what() << std::endl;
        std::cout << "Program finished with JSON file parse error!\n";
    }
}