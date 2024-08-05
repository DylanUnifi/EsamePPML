//
// Created by berkamp on 01/08/2024.
//

#ifndef GENERATE_COUNT_NGRAMS_WORD_H
#define GENERATE_COUNT_NGRAMS_WORD_H

void generate_and_count_ngrams_seq(const std::string &filename, int n, std::string& out_folder_sequential);
void generate_and_count_ngrams_par(const std::string &filename, int n, int chunk_size, int num_threads, std::string& out_folder_parallel);

#endif //GENERATE_COUNT_NGRAMS_WORD_H
