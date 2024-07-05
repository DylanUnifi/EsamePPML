from nltk.util import ngrams
from collections import Counter
from joblib import Parallel, delayed
import asyncio


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

