cmake_minimum_required(VERSION 3.17)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(SearchEngine main.cpp)

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)