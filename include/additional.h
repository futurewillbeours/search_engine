#pragma once

#include <vector>
#include <iostream>

void print_transform(std::vector<std::vector<std::pair<int, float>>> result) {
    for(int i = 0; i < result.size(); i++) {
        std::cout << i << ", ";
        for (int j = 0; j < result[i].size(); j++) {
            std::cout << j << ": doc_id: " << result[i][j].first << ", rank: " << result[i][j].second << std::endl;
        }
        std::cout << std::endl;
    }
}