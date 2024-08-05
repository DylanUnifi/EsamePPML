//
// Created by berkamp on 01/08/2024.
//

#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <string>

template <typename Func, typename... Args>
double measure_time(Func func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}
bool processInputChar(char& c);
int countWords(std::string& file_path);
std::string mergeBooks(std::string& dir, std::string& file_name);
std::string createFileWith_M_Words(std::string target_dir, std::string out_path, int M);
#endif //UTILS_H
