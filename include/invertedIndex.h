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
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    std::mutex docsAccess;
    std::mutex dictAccess;

    public:
    InvertedIndex() = default;

    void indexation(const int i) {
        docsAccess.lock();
        std::stringstream stringStream(docs[i]);
        docsAccess.unlock();
        do {
            std::string buffer;
            stringStream >> buffer;
            dictAccess.lock();
            
            if(freq_dictionary.count(buffer) == 0) freq_dictionary[buffer] = std::vector<Entry>{Entry{(size_t)i, 1}};
            else {
                bool isFound = false;
                for(int j = 0; j < freq_dictionary[buffer].size(); j++) {
                    if(i == freq_dictionary[buffer][j].doc_id) {
                        isFound = true;
                        freq_dictionary[buffer][j].count++;
                        break;
                    }
                }
                if (isFound == false) freq_dictionary[buffer].push_back(Entry{(size_t)i, 1});
            }

            dictAccess.unlock();
        } while (stringStream);
    }

    void UpdateDocumentBase (std::vector<std::string> input_docs) {
        docs = input_docs;
        std::vector<std::thread> threads;

        for(int i = 0; i < docs.size(); i++) threads.push_back(std::thread(&InvertedIndex::indexation, this, i));//indexation(i);
        
        for(int i = 0; i < docs.size(); i++) threads[i].join();
    }

    std::vector<Entry> GetWordCount(const std::string& word) {return freq_dictionary[word];}
};