#pragma once
#include "generator.h"
#include "invertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const {return (doc_id == other.doc_id && rank == other.rank);}
};

class SearchServer {
    InvertedIndex index;

    size_t WordsAmount(std::string word) {// возвращает количество слова во все документах
        size_t amount = 0;
        std::vector<Entry> freqVec = index.GetWordCount(word);
        for (auto& entry:freqVec) amount += entry.count;
        return amount;
    }

    std::vector<std::string> SortRequest(std::vector<std::string> request) {//сортирует слова по общей частоте встречаемости
        for(int i = 0; i < request.size() - 1; i++) {
            for (int j = i + 1; j < request.size(); j++) {
                if (WordsAmount(request[i]) > WordsAmount(request[j])) {
                    std::string tmp = request[i];
                    request[i] = request[j];
                    request[j] = tmp;
                }
            }
        }
        return request;
    }

    public:

    SearchServer(InvertedIndex& idx) : index(idx) {}

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput) {
        std::vector<std::vector<std::string>> requests;
        for (auto& request:queriesInput) { //разбить запросы на отдельные слова
            std::vector<std::string> vec;
            std::stringstream strstm(request);
            while(!strstm.eof()) {
                std::string buffer;
                strstm >> buffer;
                vec.push_back(buffer);
            }
            requests.push_back(vec);
        }

        for (auto& request:requests) { //оставить только уникальные слова в запросах
            std::vector<std::string> uniques;
            for(auto& word:request) {
                bool isFound = false;
                for (auto& uniqWord:uniques) if (uniqWord == word) isFound = true;
                if(!isFound) uniques.push_back(word);
            }
            request = uniques;
        }

        for (auto& request:requests) request = SortRequest(request); //сортировать слова от редких до частых

        std::vector<std::vector<size_t>> docs;
        

    }

};