from nltk import word_tokenize
from nltk.util import ngrams
from collections import Counter
import wikipediaapi
import nltk


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


# Fonction pour extraire les n-grams à partir du texte
def extract_ngrams(text, n):
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
if __name__ == "__main__":
    wikipedia_title = "Intelligence_artificielle"  # Titre de la page Wikipedia
    wikipedia_text = get_wikipedia_text(wikipedia_title)
    if wikipedia_text:
        # Extraction des bi-grams et tri-grams
        bi_grams = extract_ngrams(wikipedia_text, 2)
        tri_grams = extract_ngrams(wikipedia_text, 3)

        # Affichage des n-grams les plus fréquents
        print("Bi-grams:")
        display_most_common_ngrams(bi_grams)
        print("\nTri-grams:")
        display_most_common_ngrams(tri_grams)
