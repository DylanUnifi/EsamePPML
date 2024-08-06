#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "generate_count_ngrams_word.h"
#include "utils.h"



int main() {
    std::string books_path = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books";
    std::string out_folder_sequential = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/OutSequential/";
    std::string out_folder_parallel = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/OutParallel/";
    std::string file_path;

    // file_path= "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books/My further disillusionment in Russia.txt";
    // countWords(file_path);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/100K.txt", 100000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data//250K.txt", 250000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/500K.txt", 500000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/1M.txt", 1000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/2.5M.txt", 2500000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/5M.txt", 5000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/10M.txt", 10000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/25M.txt", 25000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/50M.txt", 50000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/100M.txt", 100000000);
    // file_path = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/200M.txt", 200000000);

    int num_runs = 10;
    int chunk_size = 1000;

    // Tester les bi-grams
    std::cout << "Benchmarking bi-grams...\n";
    std::array<int, 6> num_threads_a_tester = {1, 2, 4, 8, 16};
    std::vector<double> speedups = {/* ... vos données ici ... */};
    std::ofstream csv_file_1("/home/dylan/CLionProjects/EsameParallelProgNgrams/data/benchmark_csv/num_threads_speedup.csv");
    csv_file_1 << "NumThreads, Speedup\n";
    file_path = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/10M.txt";
    for (int num_threads : num_threads_a_tester) {
        double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        for (int i = 0; i < num_runs; i++) {

            double sequential_time = measure_time([&file_path, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(file_path, 2, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            // std::cout << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&file_path, &chunk_size, &num_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(file_path, 2, chunk_size, num_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            // std::cout << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }
        avgSpeedup = avgSpeedup / num_runs;
        std::cout << num_threads << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
        csv_file_1 << num_threads << "," << avgSpeedup << "\n";
    }

    std:: string file_path_1, file_path_2, file_path_3, file_path_4, file_path_5, file_path_6;
    // Tester les bi-grams
    file_path_1 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/1M.txt", 1000000);
    file_path_2 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/5M.txt", 5000000);
    file_path_3 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/10M.txt", 10000000);
    file_path_4 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/50M.txt", 50000000);
    file_path_5 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/100M.txt", 100000000);
    file_path_6 = createFileWith_M_Words(books_path, "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/200M.txt", 200000000);
    std::array<std::string, 6> file_paths_1 = {file_path_1, file_path_2, file_path_3, file_path_4, file_path_5, file_path_6};
    std::cout << "Benchmarking bi-grams...\n";
    std::ofstream csv_file_2("/home/dylan/CLionProjects/EsameParallelProgNgrams/data/benchmark_csv/num_word_speedup.csv");
    csv_file_2 << "chunk_size, Speedup\n";

    // std::string file_path = books_path + "10M.txt";

    for (std::string file_path : file_paths_1) {
       double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        for (int i = 0; i < num_runs; i++) {
            int n_threads = 4;

            double sequential_time = measure_time([&file_path, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(file_path, 2, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            // std::cout << i << "-" << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&file_path, &chunk_size, &n_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(file_path, 2, chunk_size, n_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            // std::cout << i << "-" << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            // std::cout << i << "-" << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }
        avgSpeedup = avgSpeedup / num_runs;
        std::cout << file_path << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
        csv_file_2 << file_path << "," << avgSpeedup << "\n";
    }

    // Tester les bi-grams
    std::cout << "Benchmarking bi-grams...\n";
    file_path_1 = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/1M.txt";
    file_path_2 = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/10M.txt";
    file_path_3 = "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/25M.txt";

    std::array<std::string, 2> file_paths_2= {file_path_1, file_path_2, };

    std::ofstream csv_file_3("/home/dylan/CLionProjects/EsameParallelProgNgrams/data/benchmark_csv/chunk_size_speedup.csv");
    csv_file_3 << "NumWords, Speedup\n";
    for (std::string num_words_file : file_paths_2) {
        double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        std::array<int, 5> chunk_sizes = {10, 10, 100, 1000, 10000};
        for (int chunk_size : chunk_sizes) {
            for (int i = 0; i < num_runs; i++) {
                int n_threads = 4;

                double sequential_time = measure_time([&num_words_file, &out_folder_sequential]() {
                    generate_and_count_ngrams_seq(num_words_file, 2, out_folder_sequential);
                });
                avgSequentialTime += sequential_time;
                // std::cout << "Sequential Time: " << sequential_time << std::endl;

                double parallel_time = measure_time([&num_words_file, &chunk_size, &n_threads, &out_folder_parallel]() {
                    generate_and_count_ngrams_par(num_words_file, 2, chunk_size, n_threads, out_folder_parallel);
                });
                avgParallelTime += parallel_time;
                // std::cout << "Parallel Time: " << parallel_time << std::endl;

                avgSpeedup += sequential_time / parallel_time;
                // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
            }
            avgSpeedup = avgSpeedup / num_runs;
            std::cout << num_words_file << " - " << chunk_size<< " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
            csv_file_3 << num_words_file << " - chunk_size= " <<chunk_size << "," << avgSpeedup << "\n";
        }
    }

    // Tester les n-grams
    std::cout << "Benchmarking n-grams...\n";
    std::array<int, 5> num_grams = {1, 2, 3, 4, 5};
    std::ofstream csv_file_4("/home/dylan/CLionProjects/EsameParallelProgNgrams/data/benchmark_csv/ngram_speedup.csv");
    csv_file_4 << "Num_grams, Speedup\n";
    for (int ngrams : num_grams) {
        double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        for (int i = 0; i < num_runs; i++) {
            int n_threads = 4;

            double sequential_time = measure_time([&file_path, &ngrams, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(file_path, ngrams, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            // std::cout << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&file_path, &chunk_size, &ngrams, &n_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(file_path, ngrams, chunk_size, n_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            // std::cout << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }
        avgSpeedup = avgSpeedup / num_runs;
        std::cout << ngrams << " : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
        csv_file_4 << ngrams << "," << avgSpeedup << "\n";
    }

    // Tester les n-grams
    std::cout << "Benchmarking n-grams...\n";
    std::array<std::string, 4> books_names = {"/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books/anna karenine.txt",
        "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books/bible.txt",
        "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books/My further disillusionment in Russia.txt",
        "/home/dylan/CLionProjects/EsameParallelProgNgrams/data/books/The Illustrated War News.txt"};
    std::ofstream csv_file_5("/home/dylan/CLionProjects/EsameParallelProgNgrams/data/benchmark_csv/books_speedup.csv");
    csv_file_5 << "Books, Speedup\n";
    for (std::string book : books_names) {
        double avgSequentialTime = 0, avgParallelTime = 0, avgSpeedup = 0;
        int num_of_word = countWords(book);
        for (int i = 0; i < num_runs; i++) {
            int n_threads = 4;

            double sequential_time = measure_time([&book, &out_folder_sequential]() {
                generate_and_count_ngrams_seq(book, 2, out_folder_sequential);
            });
            avgSequentialTime += sequential_time;
            // std::cout << "Sequential Time: " << sequential_time << std::endl;

            double parallel_time = measure_time([&book, &chunk_size, &n_threads, &out_folder_parallel]() {
                generate_and_count_ngrams_par(book, 2, chunk_size, n_threads, out_folder_parallel);
            });
            avgParallelTime += parallel_time;
            // std::cout << "Parallel Time: " << parallel_time << std::endl;

            avgSpeedup += sequential_time / parallel_time;
            // std::cout << "Speedup: " << sequential_time / parallel_time << std::endl << std::endl;
        }
        avgSpeedup = avgSpeedup / num_runs;
        std::cout << book << " with {" << num_of_word << "}" <<" words : " << "Speedup: " << avgSpeedup << std::endl << std::endl;
        csv_file_5 << book << " with {" << num_of_word << "}" <<" words : " << "," << avgSpeedup << "\n";
    }

    return 0;
}
