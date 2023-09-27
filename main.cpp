#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "nlohmann/json.hpp"

#define MAX_WORD_LENGTH 100
#define MAX_WORDS_IN_DOC 1000
#define RESOURCES_PATH "resources"

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
    
    char char_generator () {
        char c = std::rand() % 26 + 97;
        return c;
    }

    std::string word_generator() {
        int wordLength = std::rand() % MAX_WORD_LENGTH + 1;
        std::string word;
        for (int i = 0; i < wordLength; i++) word += char_generator();
        return word;
    }

    void generate_files() {

    }
};

int main() {
    std::srand(std::time(nullptr));
    auto converter = new ConverterJSON;
    auto generator = new Generator;

    for (int i = 0; i < 100; i++) {
        std::cout << generator->word_generator() << " ";
    }

    int o; std::cin >> o;
}