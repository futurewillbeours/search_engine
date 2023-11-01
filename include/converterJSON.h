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
        }
        else {
            std::vector<std::string> filesList;
            nlohmann::json config;
            configFile >> config;
            configFile.close();

            if(!config.contains("config")) throw std::runtime_error("Config file is empty");
            else {
                for (auto& el : config["files"].items()) filesList.push_back(el.value());
                std::vector<std::string> contentList;
                for(int i = 0; i < filesList.size(); i++) {
                    std::ifstream file(filesList[i]);
                    std::string str = "";
                    while(!file.eof()) {
                        std::string buffer;
                        file >> buffer;
                        str += buffer + " ";
                    }
                    contentList.push_back(str);
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
        for (auto& el : requests["requests"].items()) requestsList.push_back(el.value());
        return requestsList;
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