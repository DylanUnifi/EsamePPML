#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <omp.h>

// Fonction pour tokenizer le texte
std::vector<std::string> tokenize(const std::string& text) {
    std::istringstream stream(text);
    std::string word;
    std::vector<std::string> tokens;

    while (stream >> word) {
        tokens.push_back(word);
    }

    return tokens;
}

// Fonction pour générer les n-grams
std::vector<std::string> generate_ngrams(const std::vector<std::string>& tokens, int n) {
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

// Fonction pour compter les occurrences des n-grams
std::unordered_map<std::string, int> count_ngrams(const std::vector<std::string>& ngrams) {
    std::unordered_map<std::string, int> ngram_counts;

#pragma omp parallel for
    for (size_t i = 0; i < ngrams.size(); ++i) {
#pragma omp critical
        ngram_counts[ngrams[i]]++;
    }

    return ngram_counts;
}

// Comparateur pour le tri
bool compare_ngrams(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return b.second < a.second;
}

// Fonction pour afficher les n-grams les plus fréquents
void display_most_common_ngrams(const std::unordered_map<std::string, int>& ngram_counts, int top_n) {
    std::vector<std::pair<std::string, int>> sorted_ngrams(ngram_counts.begin(), ngram_counts.end());

    std::sort(sorted_ngrams.begin(), sorted_ngrams.end(), compare_ngrams);

    for (int i = 0; i < top_n && i < sorted_ngrams.size(); ++i) {
        std::cout << sorted_ngrams[i].first << ": " << sorted_ngrams[i].second << " occurrences\n";
    }
}

int main() {
    std::string text = "Ceci est un exemple de texte pour démontrer les bi-grams et tri-grams en utilisant OpenMP pour la parallélisation.";

    std::vector<std::string> tokens = tokenize(text);

    std::vector<std::string> bi_grams = generate_ngrams(tokens, 2);
    std::vector<std::string> tri_grams = generate_ngrams(tokens, 3);

    std::unordered_map<std::string, int> bi_gram_counts = count_ngrams(bi_grams);
    std::unordered_map<std::string, int> tri_gram_counts = count_ngrams(tri_grams);

    std::cout << "Bi-grams:\n";
    display_most_common_ngrams(bi_gram_counts, 10);

    std::cout << "\nTri-grams:\n";
    display_most_common_ngrams(tri_gram_counts, 10);

    return 0;
}
