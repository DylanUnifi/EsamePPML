from nltk import word_tokenize
from nltk.util import ngrams
from collections import Counter
from joblib import Parallel, delayed
from utils import timing_async
import wikipediaapi
import nltk
import asyncio


class PageNotFoundError(Exception):
    def __init__(self, title):
        self.title = title
        super().__init__(f"La page '{title}' n'existe pas.")


# Récupération du texte de la page Wikipedia
def get_wikipedia_text(title, lang='fr'):
    wiki = wikipediaapi.Wikipedia('Api-User-Agent', 'fr')
    try:
        page = wiki.page(title)
        return page.text
    except PageNotFoundError as e:
        print(e)


# Fonction pour prétraiter le texte et extraire les n-grams
def process_text(text, n):
    nltk.download('punkt')
    tokens = word_tokenize(text.lower())  # Tokenisation du texte
    n_grams = ngrams(tokens, n)  # Génération des n-grams
    return [' '.join(gram) for gram in n_grams]


# Fonction pour afficher les n-grams les plus fréquents
def display_most_common_ngrams(ngrams_list, top_n=10):
    ngram_counter = Counter(ngrams_list)
    print(f"Top {top_n} n-grams:")
    for ngram, count in ngram_counter.most_common(top_n):
        print(f"{ngram}: {count} occurrences")


# Exemple d'utilisation
async def main():
    wikipedia_title = "Intelligence_artificielle"  # Titre de la page Wikipedia
    wikipedia_text = get_wikipedia_text(wikipedia_title)
    if wikipedia_text:
        # Prétraitement et extraction des bi-grams et tri-grams en parallèle
        bi_grams = await asyncio.get_event_loop().run_in_executor(
            None, lambda: Parallel(n_jobs=-1)(delayed(process_text)(wikipedia_text, 2)))

        tri_grams = await asyncio.get_event_loop().run_in_executor(
            None, lambda: Parallel(n_jobs=-1)(delayed(process_text)(wikipedia_text, 3)))

        # Unpacking des résultats
        bi_grams = [gram for sublist in bi_grams for gram in sublist]
        tri_grams = [gram for sublist in tri_grams for gram in sublist]

        # Affichage des n-grams les plus fréquents
        print("Bi-grams:")
        display_most_common_ngrams(bi_grams)
        print("\nTri-grams:")
        display_most_common_ngrams(tri_grams)


# Lancer l'exécution de la fonction asynchrone principale
if __name__ == "__main__":
    asyncio.run(main())
