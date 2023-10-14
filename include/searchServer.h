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

    std::vector<RelativeIndex> RequestProcessor (std::string request) {
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

        //correct

        index.printFreqDict();

        words = SortRequest(words); //сортировать слова по мере увеличения частоты

        std::vector<size_t> docs; //заполнить вектор документов
        for (auto& word:words) {
            for(auto& entry:index.GetWordCount(word)) {
                docs.push_back(entry.doc_id);
            }
        }

        std::vector<size_t> uniqueDocs; //оставить уникальные документы в docs
        for(int i = 0; i < docs.size(); i++) {
            bool isFound = false;
            for(int j = 0; j < uniqueDocs.size(); j++) if(docs[i] == uniqueDocs[j]) isFound = true;
            if (!isFound) uniqueDocs.push_back(docs[i]);
        }
        docs = uniqueDocs;

        for (auto& word:words) { //оставить общие документы в docs
            std::vector<size_t> vec;
            for(auto& entry:index.GetWordCount(word)) vec.push_back(entry.doc_id);
            docs = GetSubset(docs, vec);
        }

        std::vector<std::pair<size_t, size_t>> absRel; //абсолюная релевантность для каждого документа
        size_t maxRel = 0; // максимальная абсолютная релевантность
        if(docs.size() != 0) { //посчитать RelativeIndex
            for (auto& doc:docs) {
                size_t absR = 0;
                for (auto& word:words) {
                    for (auto& n:index.GetWordCount(word)) {
                        if(n.doc_id == doc) absR += n.count;
                    }
                }
                absRel.push_back(std::pair<size_t, size_t>{doc, absR});
                if (absR > maxRel) maxRel = absR;
            }
        } else {}
        
        std::vector<RelativeIndex> result;
        for (auto& relPair:absRel) { //формируем вектор относительной релевантности
            RelativeIndex relInd;
            relInd.doc_id = relPair.first;
            relInd.rank = relPair.second / maxRel;
        }

        return result;
    } 

    public:

    SearchServer(InvertedIndex& idx) : index(idx) {}

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput) {
        std::vector<std::vector<RelativeIndex>> result;
        for (auto& request:queriesInput) result.push_back(RequestProcessor(request)); //mistake
        return result;
    }
};