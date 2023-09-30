#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>
#include <map>
#include "nlohmann/json.hpp"
#include "gtest/gtest.h"

//generate parameters
#define MAX_WORD_LENGTH 100
#define MAX_WORDS_IN_DOC 1000
#define PROJECT_FOLDER "../../search_engine/"
#define RESOURCES_FOLDER "../../search_engine/resources/"
#define FILE_NUMBERS 1000
#define MAX_REQUESTS 1000
#define MAX_WORDS_IN_REQUEST 10

//project parameters
#define PROJECT_NAME "SearchEngine"
#define PROJECT_VERSION "0.1"
#define MAX_RESPONSES 6

TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

std::filesystem::path resourcesPath(RESOURCES_FOLDER);

class ConverterJSON {
    public:

    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        std::vector<std::string> filesList;
        nlohmann::json config;
        std::ifstream configFile(PROJECT_FOLDER"config.json");
        configFile >> config;
        configFile.close();
        for (auto& el : config["files"].items()) filesList.push_back(el.value());
        return filesList;
    }

    int GetResponsesLimit() {
        nlohmann::json config;
        std::ifstream configFile(PROJECT_FOLDER"config.json");
        configFile >> config;
        configFile.close();
        return config["config"]["max_responses"];
    }

    std::vector<std::string> GetRequests() {
        std::vector<std::string> requestsList;
        nlohmann::json requests;
        std::ifstream requestsFile(PROJECT_FOLDER"requests.json");
        requestsFile >> requests;
        requestsFile.close();
        for (auto& el : requests["requests"].items()) requestsList.push_back(el.value());
        return requestsList;
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {//проверить
        nlohmann::json answersJSON;
        for(int i = 0; i < answers.size(); i++) {
            if (answers[i].size() == 0) answersJSON["answers"]["request" + std::to_string(i)]["result"] = false;
            else if(answers[i].size() == 1) {
                answersJSON["answers"]["request" + std::to_string(i)]["result"] = true;
                answersJSON["answers"]["request" + std::to_string(i)]["docid"] = answers[i][0].first;
                answersJSON["answers"]["request" + std::to_string(i)]["rank"] = answers[i][0].second;
            } else {
                answersJSON["answers"]["request" + std::to_string(i)]["result"] = true;
                for (int j = 0; j < answers[i].size(); j++) {
                    answersJSON["answers"]["request" + std::to_string(i)]["relevance"]["docid"] = answers[i][j].first;;
                    answersJSON["answers"]["request" + std::to_string(i)]["relevance"]["rank"] = answers[i][j].second;;
                }
            }
        }
        std::ofstream answersFile(PROJECT_FOLDER"answers.json");
        answersFile << answersJSON;
        answersFile.close();
    }
};

class InvertedIndex {
    struct Entry { size_t doc_id, count; };
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

};

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
            std::ofstream file(RESOURCES_FOLDER"file" + to_eligible_number(i) + ".txt");
            for(int j = 0; j < std::rand() % MAX_WORDS_IN_DOC + 1; j++) {
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

    void generate_state() {
        generate_resources_files();
        generate_config();
        generate_requests();
    }
};

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    
    std::srand(std::time(nullptr));
    auto generator = new Generator;
    auto converter = new ConverterJSON;
    generator -> generate_state();
    
    

    int o; std::cin >> o;
    return RUN_ALL_TESTS();
}