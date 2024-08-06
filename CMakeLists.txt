cmake_minimum_required(VERSION 3.28)
project(ProjectWorkNgrams_OpenMP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(ProjectWorkNgrams_OpenMP main.cpp
        JobsQueue.h
        generate_count_ngrams_word.cpp
        generate_count_ngrams_word.h
        generate_count_ngrams_word.h
        utils.cpp
        utils.h
        PartialHistogramsQueue.h
        HistogramCollector.h)
