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
