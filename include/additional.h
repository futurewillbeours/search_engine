#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "nlohmann/json.hpp"

#include "constants.h"
#include "structures.h"

void generateTestState(int s) {
    std::vector<std::vector<std::string>> docs(2);
    docs[0] = { "milk milk milk milk water water water",
                "milk water water",
                "milk milk milk milk milk water water water water water",
                "americano cappuccino" };
    docs[1] = { "london is the capital of great britain",
                "paris is the capital of france",
                "berlin is the capital of germany",
                "rome is the capital of italy",
                "madrid is the capital of spain",
                "lisboa is the capital of portugal",
                "bern is the capital of switzerland",
                "moscow is the capital of russia",
                "kiev is the capital of ukraine",
                "minsk is the capital of belarus",
                "astana is the capital of kazakhstan",
                "beijing is the capital of china",
                "tokyo is the capital of japan",
                "bangkok is the capital of thailand",
                "welcome to moscow the capital of russia the third rome",
                "amsterdam is the capital of netherlands",
                "helsinki is the capital of finland",
                "oslo is the capital of norway",
                "stockholm is the capital of sweden",
                "riga is the capital of latvia",
                "tallinn is the capital of estonia",
                "warsaw is the capital of poland" };
    std::filesystem::path resourcesPath(RESOURCES_FOLDER);
    std::filesystem::remove_all(resourcesPath);
    std::filesystem::create_directories(resourcesPath);
    for (int i = 0; i < docs[s].size(); i++) {
        std::ofstream file(RESOURCES_FOLDER"file" + std::to_string(i) + ".txt");
            std::stringstream strstm(docs[s][i]);
            while(!strstm.eof()) {
                std::string buffer;
                strstm >> buffer;
                file << buffer << " ";
            }
        file.close();
    }

    nlohmann::json config;
    config["config"]["name"] = PROJECT_NAME;
    config["config"]["version"] = PROJECT_VERSION;
    config["config"]["update_time"] = UPDATE_TIME;
    config["config"]["max_responses"] = MAX_RESPONSES;
    config["files"] = nlohmann::json::array();
    for (const auto & entry : std::filesystem::directory_iterator(RESOURCES_FOLDER)) config["files"].push_back(entry.path());
    std::ofstream configFile(PROJECT_FOLDER"config.json");
    configFile << config;
    configFile.close();

    const std::vector<std::string> requests = {"milk water", "sugar"};
    const std::vector<std::string> requests2 = {"moscow is the capital of russia"/*, "stockholm is the capital of sweden"*/};

    nlohmann::json requestsJSON;
    requestsJSON["requests"] = nlohmann::json::array();
    for (int i = 0; i < requests.size(); i++) requestsJSON["requests"].push_back(requests[i]);
    std::ofstream requestsFile(PROJECT_FOLDER"requests.json");
    requestsFile << requestsJSON;
    requestsFile.close();
}

void printTransform(std::vector<std::vector<std::pair<int, float>>> result) {
    for(int i = 0; i < result.size(); i++) {
        std::cout << i << ", ";
        for (int j = 0; j < result[i].size(); j++) {
            std::cout << j << ": doc_id: " << result[i][j].first << ", rank: " << result[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
}

std::map<std::string, std::vector<Entry>> printFreqDict(std::map<std::string, std::vector<Entry>> freqDictionary) {
    std::map<std::string, std::vector<Entry>>::iterator it = freqDictionary.begin();
    for(it; it != freqDictionary.end(); it++) {
        std::cout << "word: \"" << it -> first << "\"\n";
        for(auto& el:it -> second) std::cout << "doc_id: " << el.doc_id << ", count: " << el.count << std::endl;
        std::cout << std::endl;
    }
    return freqDictionary;
}

std::vector<std::vector<std::pair<int, float>>> transform(std::vector<std::vector<RelativeIndex>> search) {
    std::vector<std::vector<std::pair<int, float>>> result;
    for(auto& s1:search) {
        std::vector<std::pair<int, float>> vec;
        for (auto& s2:s1) vec.push_back(std::pair<int, float>{(int)s2.doc_id, s2.rank});
        result.push_back(vec);
    }
    return result;
}

std::string toEligNum(std::string number) {
    if (number.length() == 1) number = "00" + number;
    else if (number.length() == 2) number = "0" + number;
    return number;
}