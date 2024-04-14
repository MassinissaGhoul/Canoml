#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"



/**
 * Crée un nouveau nœud de l'arbre syntaxique.
 * @param balise Type de balise que le nœud doit représenter.
 * @param contenu Contenu textuel associé à ce nœud, si applicable.
 * @return Pointeur vers le nœud nouvellement créé.
 */
Node *creer_noeud(balise_t balise, char *contenu);

/**
 * Ajoute un nœud enfant à un nœud parent dans l'arbre syntaxique.
 * @param parent Nœud parent auquel l'enfant sera ajouté.
 * @param enfant Nœud enfant à ajouter sous le parent.
 */
void ajouter_enfant(Node *parent, Node *enfant);

/**
 * Écrit récursivement la structure de l'arbre dans un fichier de sortie, en utilisant la notation des balises.
 * @param output Fichier dans lequel écrire l'arbre.
 * @param noeud Nœud courant à écrire, fonction appelée récursivement pour chaque enfant.
 */
void ecrire_arbre(FILE *output, Node *noeud);

/**
 * Affiche l'arbre sous forme de boîtes imbriquées, utilisé pour un rendu visuel structuré du document.
 * @param noeud Nœud de départ pour l'affichage.
 * @param fichier Fichier de sortie où le rendu est écrit.
 * @param depth Profondeur actuelle dans l'arbre, utilisée pour gérer l'indentation et la largeur des boîtes.
 */
void afficher_arbre_box(Node *noeud, FILE *fichier, int depth);

/**
 * Imprime une ligne formatée avec ou sans préfixe dans une boîte de texte, respectant une largeur définie.
 * @param fichier Fichier où écrire la ligne formatée.
 * @param text Texte à imprimer dans la ligne.
 * @param width Largeur totale de la ligne, incluant les bordures de la boîte.
 * @param depth Profondeur actuelle, utilisée pour ajuster les préfixes ou indentations.
 * @param prefix Préfixe optionnel à ajouter devant le texte (par exemple, pour les listes à puces).
 */
void printFormattedLine(FILE *fichier, const char *text, int width, int depth, const char *prefix);

/**
 * Imprime les bordures d'une boîte, utilisée dans le rendu de `afficher_arbre_box`.
 * @param fichier Fichier où écrire la boîte.
 * @param width Largeur de la boîte à dessiner.
 */
void printBox(FILE *fichier, int width);

/**
 * Libère la mémoire allouée à un nœud spécifique, y compris son contenu et ses enfants.
 * @param noeud Nœud à libérer.
 */
void liberer_noeud(Node *noeud);

/**
 * Libère récursivement tous les nœuds de l'arbre, en commençant par la racine.
 * @param racine Nœud racine de l'arbre à libérer.
 */
void liberer_arbre(Node *racine);


/**
 * Affiche l'arbre syntaxique dans un fichier, en respectant la profondeur de chaque nœud.
 * @param noeud Nœud racine de l'arbre à afficher.
 * @param profondeur Profondeur actuelle dans l'arbre, utilisée pour l'indentation.
 * @param fichier Fichier de sortie où écrire l'arbre.
 */
void afficher_arbre(Node *noeud, int profondeur, FILE *fichier);

/**
 * Imprime les données d'un nœud, incluant son niveau, sa balise, et son contenu, pour le débogage.
 * @param noeud Nœud à imprimer.
 * @param level Niveau de profondeur du nœud dans l'arbre.
 * @param parent Nœud parent du nœud actuel, utilisé pour référence croisée.
 */
void printNodeData(Node *noeud, int level, Node *parent);




#endif
