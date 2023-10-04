#pragma once
#include "generator.h"
#include "invertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator == (const RelativeIndex& other) const {return (doc_id == other.doc_id && rank == other.rank);}
};

class SearchServer {
    InvertedIndex _index;
    public:
    SearchServer(InvertedIndex& idx) : _index(idx) {

    }

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
};