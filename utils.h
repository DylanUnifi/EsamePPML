//
// Created by dylan on 04/07/24.
//

#ifndef PROJECTWORKNGRAMS_OPENMP_UTILS_H
#define PROJECTWORKNGRAMS_OPENMP_UTILS_H

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>

std::vector<std::string> tokenize(const std::string& text);
bool compare_ngrams(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b);
void display_most_common_ngrams(const std::unordered_map<std::string, int>& ngram_counts, int top_n);
std::string read_file(const std::string &filename);

template <typename Func, typename... Args>
double measure_time(Func func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

#endif //PROJECTWORKNGRAMS_OPENMP_UTILS_H
