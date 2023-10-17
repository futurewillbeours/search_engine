#pragma once

#include <vector>
#include <iostream>
#include "./include/constants.h"


//generator.h
std::string to_eligible_number(int i){
        std::string eligibleNumber;
        if (i < 10) eligibleNumber = "00" + std::to_string(i);
        else if (i < 100) eligibleNumber = "0" + std::to_string(i);
        else eligibleNumber = std::to_string(i);
        return eligibleNumber;
    }

    std::string word_generator() {
        int wordLength = std::rand() % MAX_WORD_LENGTH + 1;
        std::string word;
        for (int i = 0; i < wordLength; i++) word += std::rand() % 26 + 97;
        return word;
    }

//main.cpp
void print_transform(std::vector<std::vector<std::pair<int, float>>> result) {
    for(int i = 0; i < result.size(); i++) {
        std::cout << i << ", ";
        for (int j = 0; j < result[i].size(); j++) {
            std::cout << j << ": doc_id: " << result[i][j].first << ", rank: " << result[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
}