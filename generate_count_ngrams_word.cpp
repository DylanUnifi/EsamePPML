//
// Created by berkamp on 01/08/2024.
//

#include <fstream>
#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#include <map>
#include <algorithm>

#include "generate_count_ngrams_word.h"
#include "JobsQueue.h"
#include "utils.h"
#include "HistogramCollector.h"

// Fonction séquentielle pour générer les n-grams
void generate_and_count_ngrams_seq(const std::string &filename, int n, std::string& out_folder_sequential) {
    // INPUT PHASE
    std::ifstream infile(filename);
    std::string line;
    std::string processedLine;

    std::vector<std::string> words; // vector in which we memorize all the words we read from the file
    // to store the last word in the line

    while(std::getline(infile, line)){

        /* Remove from the line what is not a letter nor a space */
        std::remove_copy_if(
            line.begin(),
            line.end(),
            std::back_inserter(processedLine),[](char& c) { return processInputChar(c); });

        size_t pos = processedLine.find(' '); // we look for spaces in order to distinguish words

        while(pos != std::string::npos){
            if(pos > 0) // in such a way we ignore double spaces
                words.push_back(processedLine.substr(0, pos));
            processedLine.erase(0, pos + 1); // erase the space as well
            pos = processedLine.find(' ');
        }

        // check if there is another word at the end of the line
        if(!processedLine.empty()) {
            std::string tmp = processedLine.substr(0, processedLine.length() - 1);
            if (!tmp.empty())
                words.push_back(tmp);
        }

        processedLine.clear();
    }

    // HISTOGRAM GENERATION
    std::map<std::string, int> histogram; // histogram in which we store the ngrams as keys, and counters as values
    std::map<std::string, int>::iterator it;

    std::string ngram;

    /* First ngram */
    for(int j=0; j < n; j++)
        ngram += words[j] + " ";
    histogram.insert(std::make_pair(ngram, 1));

    auto pos = ngram.find(' ');
    /* Following ngrams */
    for(int i=n; i < words.size(); i++) {
        ngram.erase(0, pos + 1); // we remove the first word in the previous output
        ngram += words[i] + " "; // and concatenate the last word for the following output

        it = histogram.find(ngram);
        if(it != histogram.end())
            it->second += 1; // add to the histogram
        else
            histogram.insert(std::make_pair(ngram, 1)); // insert key-value pair

        pos =  ngram.find(' '); // next space to detect the next word
    }

    // OUTPUT PHASE
    // std::ofstream outputFile;
    // outputFile.open(out_folder_sequential + std::to_string(n) + "gram_outputSequentialVersion.txt");
    //
    // for(auto& kv : histogram) // sorted, as we're using std::map
    //     outputFile << kv.first << "\t" << kv.second << std::endl;
    //
    // outputFile.close();
}


// Fonction parallèle pour générer les n-grams
void generate_and_count_ngrams_par(const std::string &filename, int n, int chunk_size, int num_threads, std::string& out_folder_parallel) {

    JobsQueue jobsQueue;    // Queue in which jobs are stored
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file !");
    }
    HistogramCollector histogramCollector;


#pragma omp parallel num_threads(num_threads) shared(file, jobsQueue, n, chunk_size, std::cout)
    {
#pragma omp single nowait
        {
            std::vector<std::string> tokens;
            std::string border[n-1];
            std::string line;
            std::string processedLine;
            std::string tmp; // to store the last word in the line
            int counter;
            size_t pos;

            counter = 0;
            while (std::getline(file, line)) {
                /* Remove from the line what is not a letter nor a space */
                std::remove_copy_if(
                        line.begin(),
                        line.end(),
                        std::back_inserter(processedLine),
                        [](char& c) { return processInputChar(c); });

                pos = processedLine.find(' ');  // we look for spaces in order to distinguish words

                while (pos != std::string::npos) {
                    if(pos > 0) { // in such a way we ignore double spaces
                        tokens.push_back(processedLine.substr(0, pos));
                        counter += 1;
                    }

                    processedLine.erase(0, pos + 1);
                    pos = processedLine.find(' ');

                    if (counter >= chunk_size) { // every chunk of words is enqueued in the jobsQueue
                        jobsQueue.enqueue(tokens);

                        for(int i=0; i < n-1; i++) // we need to save the last n-1 words as they are needed for the next chunk
                            border[i] = tokens[chunk_size - n + 1 + i];

                        tokens.clear(); // clear the local current chunk

                        for(int i=0; i < n-1; i++) // add the last n-1 words
                            tokens.push_back(border[i]);
                        counter = n-1;
                    }
                }
                // check if there is another word at the end of the line
                if(!processedLine.empty()) {
                    tmp = processedLine.substr(0, processedLine.length() - 1);
                    if (!tmp.empty()) {
                        tokens.push_back(tmp);
                        counter += 1;
                    }
                }

                processedLine.clear();
            }

            if(counter > 0){ // last chunk of data (it will be smaller in size)
                jobsQueue.enqueue(tokens);
            }

            jobsQueue.producerEnd(); // notify that the PRODUCER work is done

        }
        /* CONSUMER THREAD */
        std::vector<std::string> tokens;

        std::map<std::string, int> partialHistogram; // histogram in which we store the ngrams as keys, and counters as values
        std::map<std::string, int>::iterator it;

        std::string ngram;
        size_t pos;

        while(!jobsQueue.done()){ // until the PRODUCER hasn't finished
            if(jobsQueue.dequeue(tokens)){ // gather a job from the jobsQueue

                //std::cout << "CONSUMER is working..." << std::endl;

                ngram = "";

                /* First ngram */
                for(int j=0; j < n; j++)
                    ngram += tokens[j] + " ";

                it = partialHistogram.find(ngram);
                if(it != partialHistogram.end()) // add the ngram to the local (partial) histogram
                    it->second += 1;
                else
                    partialHistogram.insert(std::make_pair(ngram, 1));

                pos = ngram.find(' '); // detect where the first space is in order to locate the first word
                /* Following outputs */
                for(int i=n; i < tokens.size(); i++){
                    ngram.erase(0, pos + 1); // we remove the first word in the previous output
                    ngram += tokens[i] + " "; // and concatenate the last word for the following output

                    it = partialHistogram.find(ngram);
                    if(it != partialHistogram.end()) // add the ngram to the local (partial) histogram
                        it->second += 1;
                    else
                        partialHistogram.insert(std::make_pair(ngram, 1));

                    pos =  ngram.find(' ');
                }
            }
        }
        histogramCollector.addPartialHistogram(partialHistogram); // (collect the histograms and sum them only when it's time to write to file)
    }
    //histogramCollector.writeHistogramToFile(out_folder_parallel + std::to_string(n) + "gram_outputParallelVersion.txt");
}