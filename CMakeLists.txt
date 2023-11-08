cmake_minimum_required(VERSION 3.17)
project(search_engine, VERSION 1.0)

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(INCLUDE_LIST ${MY_INCLUDE_DIR}/converterJSON.h
                 ${MY_INCLUDE_DIR}/invertedIndex.h
                 ${MY_INCLUDE_DIR}/tests.h
                 ${MY_INCLUDE_DIR}/searchServer.h
                 ${MY_INCLUDE_DIR}/constants.h
                 ${MY_INCLUDE_DIR}/structures.h
                 ${MY_INCLUDE_DIR}/additional.h)

set(CMAKE_CXX_STANDARD 17)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)
add_subdirectory(src)