#pragma once
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

    void print_freq_dictionary() {
        std::map<std::string, std::vector<Entry>>::iterator it = freq_dictionary.begin();
        for (it; it != freq_dictionary.end(); it++) {
            std::cout << "Word: " << it -> first << std::endl;
            for (int i = 0; i < (it -> second).size(); i++) {
                std::cout << "doc_id: " << (it -> second)[i].doc_id << ", count: " << (it -> second)[i].count << std::endl;
            }
            std::cout << std::endl;
        }
    }

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
                    for(int j = 0; j < freq_dictionary[buffer].size(); j++) {
                        if(freq_dictionary[buffer][j].doc_id = i) freq_dictionary[buffer][j].count++;
                    }
                }
            } while (stringStream);
        }
    }

    std::vector<Entry> GetWordCount (const std::string& word);
};