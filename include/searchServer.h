#pragma once

#include <fstream>

#include "nlohmann/json.hpp"

#include "invertedIndex.h"
#include "constants.h"
#include "structures.h"

class SearchServer {
    InvertedIndex index;

    size_t wordsAmount(std::string word) {// возвращает количество слова во все документах
        size_t amount = 0;
        std::vector<Entry> freqVec = index.getWordCount(word);
        for (auto& entry:freqVec) amount += entry.count;
        return amount;
    }
    
    std::vector<RelativeIndex> requestProcessor (std::string request) {
        std::vector<std::string> words; //разбить запрос на отдельные слова
        std::stringstream strstm(request);
        while(!strstm.eof()) {
            std::string buffer;
            strstm >> buffer;
            words.push_back(buffer);
        }
        
        std::vector<std::string> unique; //оставить только уникальные слова в запросе
        for(int i = 0; i < words.size(); i++) {
            bool isFound = false;
            for(int j = 0; j < unique.size(); j++) if(words[i] == unique[j]) isFound = true;
            if (!isFound) unique.push_back(words[i]);
        }
        words = unique;

        for(int i = 0; i < words.size() - 1; i++) {//сортирует слова по общей частоте встречаемости
            for (int j = i + 1; j < words.size(); j++) {
                if (wordsAmount(words[i]) > wordsAmount(words[j])) {
                    std::string tmp = words[i];
                    words[i] = words[j];
                    words[j] = tmp;
                }
            }
        }

        std::vector<size_t> docs;//находит документы, в которых встречются слова
        for (auto& word:words) for (auto& el:index.getWordCount(word)) docs.push_back(el.doc_id);

        std::vector<size_t> tmp;//оставить только уникальные документы в docs
        for(int i = 0; i < docs.size(); i++) {
            bool isFound = false;
            for(int j = 0; j < tmp.size(); j++) if(docs[i] == tmp[j]) isFound = true;
            if (!isFound) tmp.push_back(docs[i]);
        }
        docs = tmp;
        
        std::vector<std::pair<size_t, size_t>> absRel; //абсолюная релевантность для каждого документа
        size_t maxRel = 0; // максимальная абсолютная релевантность
        for (auto& doc:docs) {//посчитать RelativeIndex
            size_t absR = 0;
            for (auto& word:words) {
                for (auto& n:index.getWordCount(word)) {
                    if(n.doc_id == doc) absR += n.count;
                }
            }
            absRel.push_back(std::pair<size_t, size_t>{doc, absR});
            if (absR > maxRel) maxRel = absR;
        }

        std::vector<RelativeIndex> result;
        for (auto& relPair:absRel) { //формируем вектор относительной релевантности
            RelativeIndex relInd;
            relInd.doc_id = relPair.first;
            relInd.rank = relPair.second / (float)maxRel;
            result.push_back(relInd);
        }

        nlohmann::json config;//считывает лимит ответов
        std::ifstream configFile(PROJECT_FOLDER"config.json");
        configFile >> config;
        configFile.close();
        int responses = config["config"]["max_responses"];

        int count = responses;//обрезает result до кол-ва ответов
        if(responses > result.size()) count = result.size();
        std::vector<RelativeIndex> temp;
        for (int i = 0; i < count; i++) temp.push_back(result[i]);
        result = temp;

        if (result.size() > 2) {//сортировка по релевантности
            for (int i = 0; i < result.size() - 1; i++) {
                for(int j = i + 1; j < result.size(); j++) {
                    if(result[i].rank < result[j].rank){
                        RelativeIndex ri = {result[i].doc_id, result[i].rank};
                        result[i] = result[j];
                        result[j] = ri;
                    }
                }
            }
        }

        return result;
    } 

    public:

    SearchServer(InvertedIndex& idx) : index(idx) {}

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queriesInput) {
        std::vector<std::vector<RelativeIndex>> result;
        for (auto& request:queriesInput) result.push_back(requestProcessor(request));
        return result;
    }
};