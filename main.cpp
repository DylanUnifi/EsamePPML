#include <chrono>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>



// Fonction pour lire un fichier texte
std::string read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file !");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


template <typename Func, typename... Args>
double measure_time(Func func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}


// Fonction pour tokeniser le texte de manière parallèle
std::vector<std::string> parallel_tokenize(const std::string &text) {
    std::vector<std::string> tokens;
    size_t length = text.size();
    int num_threads = omp_get_max_threads();
    std::vector<std::vector<std::string>> partial_tokens(num_threads);

#pragma omp parallel default(none) shared(length, num_threads, text, partial_tokens)
    {
        int tid = omp_get_thread_num();
        size_t start = (length * tid) / num_threads;
        size_t end = (length * (tid + 1)) / num_threads;

        // Ajuster les positions de début et de fin pour ne pas couper les mots
        while (start > 0 && !isspace(text[start])) {
            start--;
        }
        while (end < length && !isspace(text[end])) {
            end++;
        }

        std::stringstream ss(text.substr(start, end - start));
        std::string token;
        while (ss >> token) {
            partial_tokens[tid].push_back(token);
        }
    }

    for (const auto& pt : partial_tokens) {
        tokens.insert(tokens.end(), pt.begin(), pt.end());
    }

    return tokens;
}


// Fonction pour générer les n-grams
std::vector<std::string> generate_ngrams_par(const std::vector<std::string>& tokens, int n) {
#pragma omp master
    {
        std::cout << "Number of threads: " << omp_get_num_threads() <<std:: endl;
    }
    std::vector<std::string> ngrams;
    int size = tokens.size();

#pragma omp parallel for default(none) shared(ngrams, tokens) private(size, n)
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

#pragma omp parallel for default(none) shared(ngram_counts, ngrams)
    for (const auto & ngram : ngrams) {
#pragma omp atomic
        ngram_counts[ngram]++;
    }

    return ngram_counts;
}



int main() {

    try {

        std::string file_path_1 = "C:/Users/berka/Desktop/data/book1.txt";
        std::string text_1 = read_file(file_path_1);
        std::string file_path_2 = "C:/Users/berka/Desktop/data/book2.txt";
        std::string text_2 = read_file(file_path_1);
        std::string file_path_3 = "C:/Users/berka/Desktop/data/book3.txt";
        std::string text_3 = read_file(file_path_1);
        std::string text = text_1 + text_2 + text_3;

        // Répéter le texte pour créer des tailles de texte différentes
        std::string large_text;

        for (int i =1; i < 1000; i++) {
            large_text += text + " ";
        }
        int my_threads_num[] = {16384, 8192, 4096};
        for (int num_threads : my_threads_num) {
            omp_set_num_threads(num_threads);
            std::ofstream csv_file_bi("C:/Users/berka/Desktop/data/bigrams_benchmark.csv");
            std::ofstream csv_file_tri("C:/Users/berka/Desktop/data/trigrams_benchmark.csv");
            csv_file_bi << "Threads,Time\n";
            csv_file_tri << "Threads,Time\n";
            large_text += large_text + " ";

            // Tester les bi-grams
            std::cout << "Benchmarking bi-grams...\n";

            double parallel_time = measure_time([&large_text]() {
                const std::vector<std::string> tokens = parallel_tokenize(large_text);
                auto bi_grams = generate_ngrams_par(tokens, 2);
                auto bi_gram_counts = count_ngrams_par(bi_grams);
            });
            csv_file_bi << num_threads << "," << parallel_time << "\n";
            std::cout << "Threads: " << num_threads << ", Time bigrams: " << parallel_time << " seconds\n";

            // Tester les tri-grams
            std::cout << "Benchmarking tri-grams...\n";

            parallel_time = measure_time([&large_text] {
                const std::vector<std::string> tokens = parallel_tokenize(large_text);
                const auto tri_grams = generate_ngrams_par(tokens, 3);
                const auto tri_gram_counts = count_ngrams_par(tri_grams);
            });
            csv_file_tri << num_threads << "," << parallel_time << "\n";
            std::cout << "Threads: " << num_threads << ", Time trigrams: " << parallel_time << " seconds\n";

            csv_file_bi.close();
            csv_file_tri.close();
        }
    }catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
