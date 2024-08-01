from nltk.util import ngrams
from collections import Counter
from joblib import Parallel, delayed
import asyncio
import nltk
import time


# Fonction pour lire un fichier texte
def read_file(filename):
    with open(filename, 'r', encoding='utf-8') as file:
        return file.read()


# Assurez-vous d'avoir téléchargé le NLTK punkt tokenizer
nltk.download('punkt')


# Fonction pour tokeniser le texte
def tokenize(text):
    return nltk.word_tokenize(text)


# Fonction pour mesurer le temps d'exécution
def measure_time(func, *args):
    start = time.time()
    func(*args)
    end = time.time()
    return end - start


# Fonction asynchrone pour mesurer le temps d'exécution
async def measure_time_async(func, *args):
    start = time.time()
    await func(*args)
    end = time.time()
    return end - start


# Fonction pour générer les n-grams
def generate_ngrams(tokens, n):
    return [' '.join(gram) for gram in ngrams(tokens, n)]


# Fonction pour compter les occurrences des n-grams
def count_ngrams(ngrams_list):
    return Counter(ngrams_list)


# Fonction pour générer les n-grams en parallèle avec joblib
def generate_ngrams_joblib(tokens, n, n_jobs=4):
    chunk_size = len(tokens) // n_jobs
    chunks = [tokens[i:i + chunk_size] for i in range(0, len(tokens), chunk_size)]
    results = Parallel(n_jobs=n_jobs)(delayed(generate_ngrams)(chunk, n) for chunk in chunks)
    ngrams_list = [ngram for sublist in results for ngram in sublist]
    return ngrams_list


# Fonction pour générer les n-grams en parallèle avec asyncio
async def generate_ngrams_asyncio(tokens, n, chunk_size):
    async def process_chunk(chunk):
        return generate_ngrams(chunk, n)

    chunks = [tokens[i:i + chunk_size] for i in range(0, len(tokens), chunk_size)]
    tasks = [process_chunk(chunk) for chunk in chunks]
    results = await asyncio.gather(*tasks)
    ngrams_list = [ngram for sublist in results for ngram in sublist]
    return ngrams_list





def main():
    print('hello')
    text = read_file("C:/Users/berka/Desktop/data/book1.txt")

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


if __name__ == "__main__":
    main()
