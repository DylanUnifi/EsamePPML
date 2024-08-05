#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "generate_count_ngrams_word.h"
#include "utils.h"



int main() {
    std::string books_path = "C:/Users/berka/Desktop/data/";
    std::string out_folder_sequential = "C:/Users/berka/Desktop/data/OutSequential/";
    std::string out_folder_parallel = "C:/Users/berka/Desktop/data/OutParallel/";
    // file_path = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/1K.txt", 1000);
    // file_path = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/10K.txt", 10000);
    int num_runs = 5;

    // Tester les uni-grams
    // std::cout << "Benchmarking uni-grams...\n";
    // std::string file_path = books_path + "10M.txt";
    // int chunk_size = 10000;
    // std::array<int, 6> num_threads_a_tester = {1, 2, 4, 8, 16, 32};
    // std::vector<double> speedups = {/* ... vos données ici ... */};
    // std::ofstream csv_file("C:/Users/berka/Desktop/data/benchmark_csv/num_threads_speedup.csv");
    // csv_file << "NumThreads, Speedup\n";
    // for (int num_threads : num_threads_a_tester) {
    //     double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
    //     for (int i = 0; i < num_runs; i++) {
    //
    //         double sequential_time = measure_time([&file_path, &out_folder_sequential]() {
    //             generate_and_count_ngrams_seq(file_path, 1, out_folder_sequential);
    //         });
    //         avgSequentialTime += sequential_time;
    //         std::cout << "Sequential Time: " << sequential_time << std::endl;
    //
    //         double parallel_time = measure_time([&file_path, &chunk_size, &num_threads, &out_folder_parallel]() {
    //             generate_and_count_ngrams_par(file_path, 1, chunk_size, num_threads, out_folder_parallel);
    //         });
    //         avgParallelTime += parallel_time;
    //         std::cout << "Parallel Time: " << parallel_time << std::endl;
    //
    //         avgSpeedup += sequential_time / parallel_time;
    //         std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
    //     }
    //     avgSpeedup = avgSpeedup / num_runs;
    //     std::cout << num_threads << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
    //     csv_file << num_threads << "," << avgSpeedup << "\n";
    // }

    // Tester les bi-grams
    std::cout << "Benchmarking bi-grams...\n";
    std::array<int, 5> chunk_sizes = {10, 100, 1000, 10000, 100000};
    std::ofstream csv_file_4("C:/Users/berka/Desktop/data/benchmark_csv/chunk_size_speedup.csv");
    csv_file_4 << "chunk_size, Speedup\n";

    std::string file_path = books_path + "10M.txt";

    for (int chunk : chunk_sizes) {
       double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        for (int i = 0; i < num_runs; i++) {
            int n_threads = 4;

            double sequential_time = measure_time([&file_path, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(file_path, 2, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            std::cout << i << "-" << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&file_path, &chunk, &n_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(file_path, 2, chunk, n_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            std::cout << i << "-" << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            std::cout << i << "-" << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }
        avgSpeedup = avgSpeedup / num_runs;
        std::cout << chunk << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
        csv_file_4 << chunk << "," << avgSpeedup << "\n";
    }

    // Tester les tri-grams
    // std::cout << "Benchmarking bi-grams...\n";
    // std:: string file_path_1 = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/1K.txt", 1000);
    // std:: string file_path_2 = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/10K.txt", 10000);
    // std:: string file_path_3 = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/100K.txt", 100000);
    // std:: string file_path_4 = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/1M.txt", 10000000);
    //
    // std::array<std::string, 6> file_paths = {file_path_1, file_path_2, file_path_3, file_path_4};
    //
    // std::ofstream csv_file_2("C:/Users/berka/Desktop/data/benchmark_csv/num_word_speedup.csv");
    // csv_file_2 << "NumWords, Speedup\n";
    // for (std::string num_words_file : file_paths) {
    //     double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
    //     for (int i = 0; i < num_runs; i++) {
    //         int n_threads = 4;
    //         int chunk_size = 10000;
    //
    //         double sequential_time = measure_time([&num_words_file, &out_folder_sequential]() {
    //             generate_and_count_ngrams_seq(num_words_file, 2, out_folder_sequential);
    //         });
    //         avgSequentialTime += sequential_time;
    //         // std::cout << "Sequential Time: " << sequential_time << std::endl;
    //
    //         double parallel_time = measure_time([&num_words_file, &chunk_size, &n_threads, &out_folder_parallel]() {
    //             generate_and_count_ngrams_par(num_words_file, 2, chunk_size, n_threads, out_folder_parallel);
    //         });
    //         avgParallelTime += parallel_time;
    //         // std::cout << "Parallel Time: " << parallel_time << std::endl;
    //
    //         avgSpeedup += sequential_time / parallel_time;
    //         // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
    //     }
    //     avgSpeedup = avgSpeedup / num_runs;
    //     std::cout << num_words_file << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
    //     csv_file_2 << num_words_file << "," << avgSpeedup << "\n";
    // }

    // Tester les tri-grams
    // std::cout << "Benchmarking tri-grams...\n";
    // int chunk_size = 100;
    // std::string file_path = createFileWith_M_Words(books_path, "C:/Users/berka/Desktop/data/100K.txt", 100000);
    // std::array<int, 4> num_grams = {2, 3, 4, 5};
    // std::ofstream csv_file_3("C:/Users/berka/Desktop/data/benchmark_csv/ngram_speedup.csv");
    // csv_file_3 << "Num_grams, Speedup\n";
    // for (int ngrams : num_grams) {
    //     double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
    //     for (int i = 0; i < num_runs; i++) {
    //         int n_threads = 4;
    //
    //         double sequential_time = measure_time([&file_path, &ngrams, &out_folder_sequential]() {
    //             generate_and_count_ngrams_seq(file_path, ngrams, out_folder_sequential);
    //         });
    //         avgSequentialTime += sequential_time;
    //         // std::cout << "Sequential Time: " << sequential_time << std::endl;
    //
    //         double parallel_time = measure_time([&file_path, &chunk_size, &ngrams, &n_threads, &out_folder_parallel]() {
    //             generate_and_count_ngrams_par(file_path, ngrams, chunk_size, n_threads, out_folder_parallel);
    //         });
    //         avgParallelTime += parallel_time;
    //         // std::cout << "Parallel Time: " << parallel_time << std::endl;
    //
    //         avgSpeedup += sequential_time / parallel_time;
    //         // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
    //     }
    //     avgSpeedup = avgSpeedup / num_runs;
    //     std::cout << ngrams << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
    //     csv_file_3 << ngrams << "," << avgSpeedup << "\n";
    // }

    return 0;
}
