#pragma once

#include <thread>
#include <mutex>
#include "generator.h"

struct Entry {
    size_t doc_id, count;
    bool operator == (const Entry& other) const {return (doc_id == other.doc_id && count == other.count);}
};

class InvertedIndex {
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::mutex* dictAccess;

    void SortDict() {
        for(auto& word:freqDictionary) {
            for(int i = 0; i < word.second.size() - 1; i++) {
                for(int j = i + 1; j < word.second.size(); j++) {
                    if (word.second[i].doc_id > word.second[i + 1].doc_id) {
                        Entry tmp = word.second[i];
                        word.second[i] = word.second[j];
                        word.second[j] = tmp;
                    }
                }
            }
        }
    }

    void Indexation(const int i) {
        std::stringstream stringStream(docs[i]);
        do {
            std::string buffer;
            stringStream >> buffer;
            dictAccess -> lock();
            if(freqDictionary.count(buffer) == 0) freqDictionary[buffer] = std::vector<Entry>{Entry{(size_t)i, 1}};
            else {
                bool isFound = false;
                for(int j = 0; j < freqDictionary[buffer].size(); j++) {
                    if(i == freqDictionary[buffer][j].doc_id) {
                        isFound = true;
                        freqDictionary[buffer][j].count++;
                        break;
                    }
                }
                if (isFound == false) freqDictionary[buffer].push_back(Entry{(size_t)i, 1});
            }
            dictAccess -> unlock();
        } while (stringStream);
    }

    public:

    InvertedIndex() {
        dictAccess = new std::mutex();
    }

    void UpdateDocumentBase (std::vector<std::string> input_docs) {
        docs = input_docs;
        std::vector<std::thread> threads;
        for(int i = 0; i < docs.size(); i++) threads.push_back(std::thread(&InvertedIndex::Indexation, this, i));
        for(int i = 0; i < docs.size(); i++) threads[i].join();
        SortDict();
    }

    std::vector<Entry> GetWordCount(const std::string& word) {return freqDictionary[word];}
};