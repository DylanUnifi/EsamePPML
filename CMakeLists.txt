cmake_minimum_required(VERSION 3.28)
project(ProjectWorkNgrams_OpenMP)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(ProjectWorkNgrams_OpenMP main.cpp
        find_Ngrams_sequential.cpp
        find_Ngrams_parallel.cpp)
