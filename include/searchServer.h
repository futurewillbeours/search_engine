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

    public:

    SearchServer(InvertedIndex& idx) : index(idx) {}

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput) {
        std::vector<std::vector<RelativeIndex>> result;
        std::vector<std::string> uniqueWords;
        for (int i = 0; i < queriesInput.size(); i++) {
            bool isFound = false;
            for (int j = 0; j < uniqueWords.size(); j++) {
                if (queriesInput[i] == uniqueWords[j]) {
                    isFound = true;
                    break;
                }
            }
            if(isFound == false) uniqueWords.push_back(queriesInput[i]);
        }

        std::map<std::string, size_t> freq;
        for(auto& word:uniqueWords) {
            for(int i = 0; i < index.GetWordCount(word).size(); i++) {
                if (freq.count(word) == 0) freq[word] = index.GetWordCount(word)[i].count;
                else freq[word] += index.GetWordCount(word)[i].count;
            }
        }

        for (int i = 0; i < uniqueWords.size() - 1; i++) {
            for (int j = i + 1; j < uniqueWords.size(); j++) {
                if (freq[uniqueWords[i]] > freq[uniqueWords[j]]) {
                    std::string tmp = uniqueWords[i];
                    uniqueWords[i] = uniqueWords[j];
                    uniqueWords[j] = tmp;
                }
            }
        }

        return result;
    }
};