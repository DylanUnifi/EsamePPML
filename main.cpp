#include "utils.h"
#include "find_Ngrams.h"



int main() {

    try {
        std::string text = read_file("/home/dylan/CLionProjects/ProjectWorkNgrams_OpenMP/data/book2.txt");

        // Répéter le texte pour créer des tailles de texte différentes
        std::string large_text;
        for (int i = 0; i < 10; ++i) {
            large_text += text + " ";

            std::vector<std::string> tokens = tokenize(large_text);

            // Tester les bi-grams
            std::cout << "Benchmarking bi-grams...\n";

            double sequential_time = measure_time([&tokens]() {
                auto bi_grams = generate_ngrams(tokens, 2);
                auto bi_gram_counts = count_ngrams(bi_grams);
            });

            double parallel_time = measure_time([&tokens]() {
                auto bi_grams = generate_ngrams_par(tokens, 2);
                auto bi_gram_counts = count_ngrams_par(bi_grams);
            });

            std::cout << "Sequential bi-grams time: " << sequential_time << " seconds\n";
            std::cout << "Parallel bi-grams time: " << parallel_time << " seconds\n";

            // Tester les tri-grams
            std::cout << "\nBenchmarking tri-grams...\n";

            sequential_time = measure_time([&tokens]() {
                auto tri_grams = generate_ngrams(tokens, 3);
                auto tri_gram_counts = count_ngrams(tri_grams);
            });

            parallel_time = measure_time([&tokens]() {
                auto tri_grams = generate_ngrams_par(tokens, 3);
                auto tri_gram_counts = count_ngrams_par(tri_grams);
            });

            std::cout << "Sequential tri-grams time: " << sequential_time << " seconds\n";
            std::cout << "Parallel tri-grams time: " << parallel_time << " seconds\n";

        }
    }catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

       /* std::cout << "Bi-grams: sequential\n";
        display_most_common_ngrams(bi_gram_counts_seq, 10);

        std::cout << "Bi-grams: parallel\n";
        display_most_common_ngrams(bi_gram_counts_par, 10);

        std::cout << "\nTri-grams parallel:\n";
        display_most_common_ngrams(tri_gram_counts_par, 10);

        std::cout << "\nTri-grams sequential:\n";
        display_most_common_ngrams(tri_gram_counts_seq, 10);*/
    return 0;
}
