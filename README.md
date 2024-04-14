# Projet nanoml

## Description
Le projet nanoml est développé dans le cadre du cours d'Algorithmes avancés pour la Licence Sciences du Numérique. Il vise à construire un analyseur syntaxique pour un format de document simplifié, inspiré par HTML, appelé nanoml. Ce README décrit les principales composantes du projet, y compris les fonctions clés implémentées en C.
L'affochage se fait dans bloc.txt

## Fonctions

### Ouverture et Fermeture des Fichiers
- `void ouverture(element *e, char *nom_fichier)`: Ouvre un fichier pour la lecture et initialise la structure `element`.
- `void fermeture(element *e)`: Ferme les fichiers ouverts et finalise l'analyse en écrivant l'arbre dans un fichier de sortie.

### Analyse Syntaxique
- `void lire_caractere(element *e)`: Lit le prochain caractère du fichier source.
- `void passer_espace(element *e)`: Ignore les espaces blancs dans le flux de données.
- `void update_balise_actuelle(element *e, char *token)`: Met à jour la balise actuelle en fonction du token lu.
- `void lire_token(element *e)`: Lit le prochain token jusqu'à rencontrer un '>'.

### Gestion des Tokens
- `void consommer(element *e, char caractere)`: Consomme un caractère spécifique et avance dans le flux de lecture.
- `void consommer_token(element *e, char *token)`: Consomme une chaîne de caractères spécifique et avance dans le flux de lecture.

### Construction et Affichage de l'Arbre
- `Node *creer_noeud(balise_t balise, char *contenu)`: Crée un nouveau nœud de l'arbre syntaxique.
- `void ajouter_enfant(Node *parent, Node *enfant)`: Ajoute un enfant à un nœud parent dans l'arbre.
- `void ecrire_arbre(FILE *output, Node *noeud)`: Écrit la structure de l'arbre dans un fichier.
- `void afficher_arbre(Node *noeud, int profondeur, FILE *fichier)`: Affiche l'arbre syntaxique dans un fichier.

### Affichage Avancé
- `void afficher_arbre_box(Node *noeud, FILE *fichier, int depth)`: Affiche l'arbre sous forme de boîtes imbriquées.
- `void printFormattedLine(FILE *fichier, const char *text, int width, int depth, const char *prefix)`: Imprime une ligne formatée dans une boîte de texte.
- `void printBox(FILE *fichier, int width)`: Imprime les bordures d'une boîte.

### Libération de la Mémoire
- `void liberer_noeud(Node *noeud)`: Libère la mémoire allouée à un nœud spécifique.
- `void liberer_arbre(Node *racine)`: Libère récursivement tous les nœuds de l'arbre.

## Compilation et Exécution
Pour compiler et exécuter le projet, utilisez les commandes suivantes dans votre terminal (assurez-vous que GCC est installé):

```bash
make all
./canoml
