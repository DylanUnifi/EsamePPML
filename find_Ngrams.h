//
// Created by dylan on 04/07/24.
//

#ifndef PROJECTWORKNGRAMS_OPENMP_FIND_NGRAMS_H
#define PROJECTWORKNGRAMS_OPENMP_FIND_NGRAMS_H

#include <omp.h>
#include <vector>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> count_ngrams_par(const std::vector<std::string>& ngrams);
std::vector<std::string> generate_ngrams_par(const std::vector<std::string>& tokens, int n);

std::unordered_map<std::string, int> count_ngrams(const std::vector<std::string>& ngrams);
std::vector<std::string> generate_ngrams(const std::vector<std::string>& tokens, int n);
#endif //PROJECTWORKNGRAMS_OPENMP_FIND_NGRAMS_H
