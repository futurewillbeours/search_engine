#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "nlohmann/json.hpp"

//generate parameters
#define MAX_WORD_LENGTH 100
#define MAX_WORDS_IN_DOC 1000
#define PROJECT_FOLDER "../../search_engine/"
#define RESOURCES_FOLDER "../../search_engine/resources/"
#define FILE_NUMBERS 100
#define MAX_REQUESTS 1000
#define MAX_WORDS_IN_REQUEST 10

//project parameters
#define PROJECT_NAME "SearchEngine"
#define PROJECT_VERSION "0.1"
#define MAX_RESPONSES 6

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

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};

class Generator {
    public:

    std::string word_generator() {
        int wordLength = std::rand() % MAX_WORD_LENGTH + 1;
        std::string word;
        for (int i = 0; i < wordLength; i++) word += std::rand() % 26 + 97;
        return word;
    }

    void generate_resources_files() {
        std::filesystem::remove_all(resourcesPath);
        std::filesystem::create_directories(resourcesPath);
        for (int i = 0; i < std::rand() % FILE_NUMBERS + 1; i++) {
            std::ofstream file(RESOURCES_FOLDER"file" + std::to_string(i) + ".txt");
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

    void generate_requests() {//не более 1000 запросов от 1 до 10 слов
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

int main() {
    std::srand(std::time(nullptr));
    auto generator = new Generator;
    generator -> generate_state();
    auto converter = new ConverterJSON;
    auto requests = converter->GetRequests();

    for (auto& request:requests) std::cout << request << std::endl;

    int o; std::cin >> o;
}