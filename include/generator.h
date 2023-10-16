#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

//generator parameters
#define MAX_WORD_LENGTH 3 //100 by default
#define MAX_WORDS_IN_DOC 1000 //1000 by default
#define PROJECT_FOLDER "../../search_engine/"
#define RESOURCES_FOLDER "../../search_engine/resources/"
#define FILE_NUMBERS 10 //1000 by default
#define MAX_REQUESTS 1000
#define MAX_WORDS_IN_REQUEST 10

//project parameters
#define PROJECT_NAME "search_engine"
#define PROJECT_VERSION "0.1"
#define MAX_RESPONSES 5

std::filesystem::path resourcesPath(RESOURCES_FOLDER);

class Generator {
    public:

    std::string to_eligible_number(int i){
        std::string eligibleNumber;
        if (i < 10) eligibleNumber = "00" + std::to_string(i);
        else if (i < 100) eligibleNumber = "0" + std::to_string(i);
        else eligibleNumber = std::to_string(i);
        return eligibleNumber;
    }

    std::string word_generator() {
        int wordLength = std::rand() % MAX_WORD_LENGTH + 1;
        std::string word;
        for (int i = 0; i < wordLength; i++) word += std::rand() % 26 + 97;
        return word;
    }

    void generate_resources_files() {
        std::filesystem::remove_all(resourcesPath);
        std::filesystem::create_directories(resourcesPath);
        int file_numbers = std::rand() % FILE_NUMBERS + 1;
        for (int i = 0; i < file_numbers; i++) {
            std::ofstream file(RESOURCES_FOLDER"file" + to_eligible_number(i + 1) + ".txt");
            for(int j = 1; j <= std::rand() % MAX_WORDS_IN_DOC + 1; j++) {
                std::string buffer = word_generator();
                file << buffer;
                file << " ";
            }
            file.close();
        }
    }

    void generate_config() {
        nlohmann::json config;
        config["config"]["name"] = PROJECT_NAME;
        config["config"]["version"] = PROJECT_VERSION;
        config["config"]["max_responses"] = MAX_RESPONSES;
        config["files"] = nlohmann::json::array();
        for (const auto & entry : std::filesystem::directory_iterator(RESOURCES_FOLDER)) config["files"].push_back(entry.path());
        std::ofstream configFile(PROJECT_FOLDER"config.json");
        configFile << config;
        configFile.close();
    }

    void generate_requests() {
        nlohmann::json requests;
        requests["requests"] = nlohmann::json::array();
        for (int i = 0; i < std::rand() % MAX_REQUESTS + 1; i++) {
            std::string request;
            for (int j = 0; j < std::rand() % MAX_WORDS_IN_REQUEST + 1; j++) request = request + word_generator() + " ";
            requests["requests"].push_back(request);
        }
        std::ofstream requestsFile(PROJECT_FOLDER"requests.json");
        requestsFile << requests;
        requestsFile.close();
    }
    
    void generate_random_state() {
        generate_resources_files();
        generate_config();
        generate_requests();
    }

    void generate_test_state() {
        const std::vector<std::string> docs = {"london is the capital of great britain",
                                            "paris is the capital of france",
                                            "berlin is the capital of germany",
                                            "rome is the capital of italy",
                                            "madrid is the capital of spain",
                                            "lisboa is the capital of portugal",
                                            "bern is the capital of switzerland",
                                            "moscow is the capital of russia",//7
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
                                            "warsaw is the capital of poland"};
        std::filesystem::remove_all(resourcesPath);
        std::filesystem::create_directories(resourcesPath);
        for (int i = 0; i < docs.size(); i++) {
            std::ofstream file(RESOURCES_FOLDER"file" + std::to_string(i) + ".txt");
                std::stringstream strstm(docs[i]);
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
        config["config"]["max_responses"] = MAX_RESPONSES;
        config["files"] = nlohmann::json::array();
        for (const auto & entry : std::filesystem::directory_iterator(RESOURCES_FOLDER)) config["files"].push_back(entry.path());
        std::ofstream configFile(PROJECT_FOLDER"config.json");
        configFile << config;
        configFile.close();

        const std::vector<std::string> requests = {"moscow is the capital of russia"/*, "stockholm is the capital of sweden"*/};

        nlohmann::json requestsJSON;
        requestsJSON["requests"] = nlohmann::json::array();
        for (int i = 0; i < requests.size(); i++) requestsJSON["requests"].push_back(requests[i]);
        std::ofstream requestsFile(PROJECT_FOLDER"requests.json");
        requestsFile << requestsJSON;
        requestsFile.close();
    }
};