//
// Created by dylan on 01/07/24.
//

#include "find_Ngrams.h"


// Fonction pour générer les n-grams
std::vector<std::string> generate_ngrams_par(const std::vector<std::string>& tokens, int n) {
    std::vector<std::string> ngrams;
    int size = tokens.size();

#pragma omp parallel for
    for (int i = 0; i <= size - n; ++i) {
        std::string ngram = tokens[i];
        for (int j = 1; j < n; ++j) {
            ngram += " " + tokens[i + j];
        }
#pragma omp critical
        ngrams.push_back(ngram);
    }

    return ngrams;
}

// Fonction pour générer les n-grams
std::vector<std::string> generate_ngrams(const std::vector<std::string>& tokens, int n) {
    std::vector<std::string> ngrams;
    int size = tokens.size();

    for (int i = 0; i <= size - n; ++i) {
        std::string ngram = tokens[i];
        for (int j = 1; j < n; ++j) {
            ngram += " " + tokens[i + j];
        }
        ngrams.push_back(ngram);
    }

    return ngrams;
}

// Fonction pour compter les occurrences des n-grams
std::unordered_map<std::string, int> count_ngrams_par(const std::vector<std::string>& ngrams) {
    std::unordered_map<std::string, int> ngram_counts;

#pragma omp parallel for
    for (size_t i = 0; i < ngrams.size(); ++i) {
#pragma omp critical
        ngram_counts[ngrams[i]]++;
    }

    return ngram_counts;
}

// Fonction pour compter les occurrences des n-grams
std::unordered_map<std::string, int> count_ngrams(const std::vector<std::string>& ngrams) {
    std::unordered_map<std::string, int> ngram_counts;

    for (size_t i = 0; i < ngrams.size(); ++i) {
        ngram_counts[ngrams[i]]++;
    }

    return ngram_counts;
}
