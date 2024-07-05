//
// Created by dylan on 04/07/24.
//

#include "utils.h"


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

// Fonction pour lire un fichier texte
std::string read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
