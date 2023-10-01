#pragma once

#include <thread>
#include "generator.h"

struct Entry {
    size_t doc_id, count;
    bool operator == (const Entry& other) const {return (doc_id == other.doc_id && count == other.count);}
};

class InvertedIndex {
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

    public:
    InvertedIndex() = default;

    void UpdateDocumentBase (std::vector<std::string> input_docs) {
        docs = input_docs;
        for(int i = 0; i < docs.size(); i++) {
            std::stringstream stringStream(docs[i]);
            do {
                std::string buffer;
                stringStream >> buffer;
                if(freq_dictionary.count(buffer) == 0) {
                    std::vector<Entry> vec;
                    Entry entry;
                    entry.count = 1;
                    entry.doc_id = i;
                    vec.push_back(entry);
                    freq_dictionary[buffer] = vec;
                } else {
                    bool isFound = false;
                    for(int j = 0; j < freq_dictionary[buffer].size(); j++) {
                        if(i == freq_dictionary[buffer][j].doc_id) {
                            isFound = true;
                            freq_dictionary[buffer][j].count++;
                        }
                    }
                    if (isFound == false) {
                        std::vector<Entry> vec;
                        Entry entry;
                        entry.count = 1;
                        entry.doc_id = i;
                        freq_dictionary[buffer].push_back(entry);
                    }
                }
            } while (stringStream);
        }
    }

    std::vector<Entry> GetWordCount (const std::string& word) {return freq_dictionary[word];}
};