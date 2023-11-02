#pragma once

#include <vector>
#include <string>

#include "nlohmann/json.hpp"
#include "constants.h"

class ConverterJSON {
    public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments() {
        std::ifstream configFile(PROJECT_FOLDER"config.json");
        if(!configFile) {
            throw std::runtime_error("Config file is missing");
            configFile.close();
        } else {
            std::vector<std::string> filesList;
            nlohmann::json config;
            configFile >> config;
            configFile.close();
            if(!config.contains("config")) throw std::runtime_error("Config file is empty");
            else {
                if(config["config"].contains("name")) std::cout << "Search engine name: " << config["config"]["name"] << std::endl;
                if(PROJECT_VERSION != config["config"]["version"]) std::cout << "config.json has incorrect file version" << std::endl;
                int filesCount = 0;
                for (auto& el : config["files"].items()) {
                    if (filesCount < FILE_NUMBERS) filesList.push_back(el.value());
                    filesCount++;
                }
                if (filesCount > FILE_NUMBERS) std::cout << "More than " << FILE_NUMBERS << " were skipped!\n";
                std::vector<std::string> contentList;
                for(int i = 0; i < filesList.size(); i++) {
                    std::ifstream file(filesList[i]);
                    if(!file) std::cout << filesList[i] << " doesnt exist!\n";
                    else {
                        std::string str = "";
                        int count = 0;
                        while(!file.eof() || count < MAX_WORDS_IN_DOC) {
                            std::string buffer;
                            file >> buffer;
                            if (buffer.length() <= MAX_WORD_LENGTH) str += buffer + " ";
                            else std::cout << "Word " << buffer << " skipped: more than " << MAX_WORD_LENGTH << " characters!\n";
                            count++;
                        }
                        if (count > MAX_WORDS_IN_DOC) std::cout << "More than " << MAX_WORDS_IN_DOC << " in docunents were skipped!\n";
                        contentList.push_back(str);
                    }
                    file.close();
                }
            return contentList;
            }
        }
    }

    int GetResponsesLimit() {
        nlohmann::json config;
        std::ifstream configFile(PROJECT_FOLDER"config.json");
        configFile >> config;
        configFile.close();
        if (config["config"].contains("max_responses")) return config["config"]["max_responses"];
        else return 5;
    }

    std::vector<std::string> GetRequests() {
        std::vector<std::string> requestsList;
        nlohmann::json requests;
        std::ifstream requestsFile(PROJECT_FOLDER"requests.json");
        requestsFile >> requests;
        requestsFile.close();
        int requestsCount = 0;
        for (auto& el : requests["requests"].items()) {
            if (requestsCount < MAX_REQUESTS) requestsList.push_back(el.value());
            requestsCount++;
        }
        if (requestsCount > MAX_REQUESTS) std::cout << "More than " << MAX_REQUESTS << " requests were skipped!\n";
        std::vector<std::string> requestsListNew;
        for (auto& req:requestsList) {
            std::stringstream strstm(req);
            std::string buffer;
            std::string str;
            int wordsCount = 0;
            while (!strstm.eof()) {
                strstm >> buffer;
                if (wordsCount < MAX_WORDS_IN_REQUEST) {
                    str += buffer;
                    str += " ";
                }
                wordsCount++;
            }
            requestsListNew.push_back(str);
            if (wordsCount > MAX_WORDS_IN_REQUEST) std::cout << "More than " << MAX_WORDS_IN_REQUEST << " words in request were skipped!\n";
        }
        return requestsListNew;
    }

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
        nlohmann::json answersJSON;
        int count = this -> GetResponsesLimit();
        for(int i = 0; i < answers.size(); i++) {
            if (answers[i].size() == 0) answersJSON["answers"]["request" + std::to_string(i)]["result"] = false;
            else if(answers[i].size() == 1) {
                answersJSON["answers"]["request" + std::to_string(i)]["result"] = true;
                answersJSON["answers"]["request" + std::to_string(i)]["docid"] = answers[i][0].first;
                answersJSON["answers"]["request" + std::to_string(i)]["rank"] = answers[i][0].second;
            } else {
                if (answers[i].size() < count) count = answers[i].size();
                answersJSON["answers"]["request" + std::to_string(i)]["result"] = true;
                for (int j = 0; j < count; j++) {
                    answersJSON["answers"]["request" + std::to_string(i)]["relevance"]["docid"].push_back(answers[i][j].first);
                    answersJSON["answers"]["request" + std::to_string(i)]["relevance"]["rank"].push_back(answers[i][j].second);
                }
            }
        }
        std::ofstream answersFile(PROJECT_FOLDER"answers.json");
        answersFile << answersJSON;
        answersFile.close();
    }
};