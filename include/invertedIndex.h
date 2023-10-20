#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>

#include "structures.h"
#include "additional.h"

class InvertedIndex {
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
    std::mutex* dictAccess;

    void Indexation(const int i) {
        std::stringstream stringStream(docs[i]);
        do {
            std::string buffer;
            stringStream >> buffer;
            if (buffer == "") break;
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

    InvertedIndex() {dictAccess = new std::mutex();}

    void UpdateDocumentBase (std::vector<std::string> input_docs) {
        docs = input_docs;
        for (auto& doc:docs) std::cout << doc << std::endl;

        std::vector<std::thread> threads;
        for(int i = 0; i < docs.size(); i++) threads.push_back(std::thread(&InvertedIndex::Indexation, this, i));
        for(int i = 0; i < docs.size(); i++) threads[i].join();

        for (auto it = freqDictionary.begin(); it != freqDictionary.end(); it++) {
            if(it -> second.size() > 1) {
                for(int i = 0; i < it -> second.size() - 1; i++) {
                    for(int j = i + 1; j < it -> second.size(); j++) {
                        if((it -> second)[i].doc_id > (it -> second)[j].doc_id) {
                            Entry tmp;
                            tmp.doc_id = it -> second[i].doc_id;
                            tmp.count = it -> second[i].count;
                            (it -> second)[i] = (it -> second)[j];
                            (it -> second)[j] = tmp;
                        }
                    }
                }
            }
            
        }
    }

    std::vector<Entry> GetWordCount(const std::string& word) {return freqDictionary[word];}
};