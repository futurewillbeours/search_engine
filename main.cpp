#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "nlohmann/json.hpp"

#define MAX_WORD_LENGTH 100
#define MAX_WORDS_IN_DOC 1000
#define RESOURCES_FOLDER "../../search_engine/resources/"
#define FILE_NUMBERS 100

class ConverterJSON {
    public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
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

    void generate_files() {
        std::filesystem::path resourcesPath(RESOURCES_FOLDER);
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
};

int main() {
    std::srand(std::time(nullptr));
    auto converter = new ConverterJSON;
    auto generator = new Generator;

    generator->generate_files();

    //int o; std::cin >> o;
}