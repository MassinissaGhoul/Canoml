#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"


/**
 * Ouvre un fichier donné pour la lecture et initialise l'analyse.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param nom_fichier Nom du fichier à ouvrir pour la lecture.
 */
void ouverture(element *e, char *nom_fichier);

/**
 * Ferme les fichiers ouverts et finalise l'analyse en écrivant l'arbre dans un fichier de sortie.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void fermeture(element *e);

/**
 * Lit le caractère suivant du fichier source.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void lire_caractere(element *e);

/**
 * Ignore les espaces blancs dans le flux de données jusqu'à trouver un caractère significatif.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void passer_espace(element *e);

/**
 * Met à jour la balise actuelle en fonction du token lu.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param token Token lu qui représente une balise ou un contenu.
 */
void update_balise_actuelle(element *e, char *token);

/**
 * Lit le prochain token du fichier jusqu'à rencontrer un '>'.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void lire_token(element *e);

/**
 * Consomme un caractère spécifique et avance dans le flux de lecture.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param caractere Caractère à consommer.
 */
void consommer(element *e, char caractere);

/**
 * Consomme une chaîne de caractères (token) spécifique et avance dans le flux de lecture.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param token Chaîne de caractères à consommer.
 */
void consommer_token(element *e, char *token);

/**
 * Analyse le texte enrichi, déclenchant l'analyse de l'ensemble du document.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void texte_enrichi(element *e);

/**
 * Analyse le document, créant un nœud racine et gérant le contenu du document.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void document(element *e);

/**
 * Analyse le contenu entre les balises, gérant les structures imbriquées.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param balise_contenante Balise qui contient le contenu actuel.
 */
void contenu(element *e, int balise_contenante);

/**
 * Analyse une section, traitant son contenu.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void section(element *e);

/**
 * Analyse un titre, traitant le texte qu'il contient.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void titre(element *e);

/**
 * Analyse et traite le texte dans différentes structures (titres, items, etc.).
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void texte(element *e);

/**
 * Analyse une liste, traitant chaque item qu'elle contient.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void liste(element *e);

/**
 * Analyse un item de liste, gérant le contenu ou les sous-listes.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void item(element *e);

/**
 * Analyse les annexes, traitant leur contenu comme une partie du document principal.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void annexes(element *e);

/**
 * Traite un mot enrichi, appliquant une importance spécifique ou traitant comme du texte simple.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void mot_enrichi(element *e);

/**
 * Analyse et traite un mot important, souvent mis en évidence.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void mot_important(element *e);

/**
 * Traite un mot simple, ajoutant du texte au nœud courant de l'arbre.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 */
void mot_simple(element *e);

/**
 * Ferme la balise en cours de traitement et remonte dans la hiérarchie de l'arbre.
 * @param e Pointeur vers la structure de l'élément gérant l'état de l'analyse.
 * @param balise_contenante Balise contenant la structure en cours.
 */
void fermeture_balise(element *e, int balise_contenante);

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


/**
 * Identifie le type de balise à partir d'un token de chaîne de caractères.
 * Compare le token avec un tableau prédéfini de chaînes représentant les balises valides du format nanoml.
 *
 * @param token La chaîne de caractères représentant une balise potentielle à identifier.
 * @return Le type de balise correspondant (balise_t) si le token est reconnu, sinon retourne BALISE_INCONNUE.
 */
balise_t get_balise(const char *token);




#endif
