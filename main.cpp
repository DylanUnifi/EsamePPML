#include <iostream>
#include <string>

#include "generate_count_ngrams_word.h"
#include "utils.h"



int main() {
    std::string books_path = "C:/Users/berka/Desktop/data/";
    // std::string merge_name = "MergedBooks.txt";
    // std::string file_path = mergeBooks(books_path, merge_name);
    std::string file_path = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/5M.txt", 5000000);
    // std::string file_path = "C:/Users/berka/Desktop/data/book2.txt";

    std::string out_folder_sequential = "C:/Users/berka/Desktop/data/OutSequential/";
    std::string out_folder_parallel = "C:/Users/berka/Desktop/data/OutParallel/";

    int num_runs = 10;
    int chunk_size = 10000;
    int n_threads = 8;

    double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;

        for (int i = 0; i < num_runs; i++) {

            // Tester les bi-grams
            std::cout << "Benchmarking bi-grams...\n";
            double sequential_time = measure_time([&file_path, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(file_path, 2, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            std::cout << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&file_path, &chunk_size, &n_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(file_path, 2, chunk_size, n_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            std::cout << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }

    avgSequentialTime = avgSequentialTime / num_runs;
    avgParallelTime = avgParallelTime / num_runs;
    avgSpeedup = avgSpeedup / num_runs;

    std::cout << std::endl << "Average Sequential Time: " << avgSequentialTime << std::endl;
    std::cout << "Average Parallel Time: " << avgParallelTime << std::endl;
    std::cout << "Average Speedup: " << avgSpeedup << std::endl;
    std::cout << "Average Speedup: " << avgSequentialTime / avgParallelTime << std::endl;

    std::cout << std::endl << std::endl;

    return 0;
}
