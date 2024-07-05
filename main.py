from utils import read_file,  tokenize, measure_time, measure_time_async
import asyncio
from find_Ngrams import count_ngrams, generate_ngrams, generate_ngrams_joblib, generate_ngrams_asyncio


def main():
    text = read_file("/home/dylan/CLionProjects/ProjectWorkNgrams_OpenMP/data/book1.txt")

    for i in range(1, 10):

        # Répéter le texte pour créer des tailles de texte différentes
        large_text = text * i
        tokens = tokenize(large_text)

        # Benchmarking bi-grams
        print("Benchmarking bi-grams...")

        sequential_time = measure_time(lambda: count_ngrams(generate_ngrams(tokens, 2)))
        joblib_time = measure_time(lambda: count_ngrams(generate_ngrams_joblib(tokens, 2, 8)))
        asyncio_time = asyncio.run(measure_time_async(generate_ngrams_asyncio, tokens, 2, len(tokens)//4))

        print(f"Sequential bi-grams time: {sequential_time:.4f} seconds")
        print(f"Joblib bi-grams time: {joblib_time:.4f} seconds")
        print(f"Asyncio bi-grams time: {asyncio_time:.4f} seconds")

        # Benchmarking tri-grams
        print("\nBenchmarking tri-grams...")

        sequential_time = measure_time(lambda: count_ngrams(generate_ngrams(tokens, 3)))
        joblib_time = measure_time(lambda: count_ngrams(generate_ngrams_joblib(tokens, 3, 8)))
        asyncio_time = asyncio.run(measure_time_async(generate_ngrams_asyncio, tokens, 3, len(tokens)//4))

        print(f"Sequential tri-grams time: {sequential_time:.4f} seconds")
        print(f"Joblib tri-grams time: {joblib_time:.4f} seconds")
        print(f"Asyncio tri-grams time: {asyncio_time:.4f} seconds")
        print('\n')


if __name__ == "__main__":
    main()
