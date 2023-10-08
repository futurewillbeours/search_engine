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

    std::vector<size_t> GetSubset(std::vector<size_t> docs, std::vector<size_t> vec) {//оставить только значения vec в docs
        for(int i = 0; i < docs.size(); i++) {
            bool isFind = false;
            for (int j = 0; j < vec.size(); j++) if(docs[i] == vec[j]) isFind = true;
            if(!isFind) {
                for(std::vector<size_t>::iterator it = docs.begin(); it != docs.end(); ++it) {
                    if(*it == docs[i]) {
                        docs.erase(it);
                        break;
                    }
                }
            }
        }
        return docs;
    }

    void RequestProcessor (std::string request) {
        //разбить запрос на отдельные слова
        std::vector<std::string> words;
        std::stringstream strstm(request);
        while(!strstm.eof()) {
            std::string buffer;
            strstm >> buffer;
            words.push_back(buffer);
        }

        //оставить только уникальные слова в запросе
        std::vector<std::string> unique;
        for(int i = 0; i < words.size(); i++) {
            bool isFound = false;
            for(int j = 0; j < unique.size(); j++) if(words[i] == unique[j]) isFound = true;
            if (!isFound) unique.push_back(words[i]);
        }
        words = unique;

        //сортировать слова по мере увеличения частоты
        words = SortRequest(words);

        //заполнить вектор документов
        std::vector<size_t> docs;
        for (auto& word:words) {
            for(auto& entry:index.GetWordCount(word)) {
                docs.push_back(entry.doc_id);
            }
        }

        //оставить уникальные документы в docs
        std::vector<size_t> uniqueDocs;
        for(int i = 0; i < docs.size(); i++) {
            bool isFound = false;
            for(int j = 0; j < uniqueDocs.size(); j++) if(docs[i] == uniqueDocs[j]) isFound = true;
            if (!isFound) uniqueDocs.push_back(docs[i]);
        }
        docs = uniqueDocs;

        //оставить общие документы в docs
        for (auto& word:words) {
            std::vector<size_t> vec;
            for(auto& entry:index.GetWordCount(word)) vec.push_back(entry.doc_id);
            docs = GetSubset(docs, vec);
        }

        
    } 

    public:

    SearchServer(InvertedIndex& idx) : index(idx) {}

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput) {
        std::vector<std::vector<RelativeIndex>> result;
        for (auto& request:queriesInput) RequestProcessor(request);

        return result;
    }

};