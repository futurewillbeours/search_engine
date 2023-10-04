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

        
    }
};