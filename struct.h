#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Enumération des différents types de balises utilisés dans le traitement du format nanoml.
 * Ces identifiants permettent de classifier et de manipuler les éléments structuraux du document.
 */
typedef enum
{
    BALISE_INCONNUE,         // Balise non reconnue par le système.
    BALISE_DOCUMENT,         // Balise indiquant le début d'un document.
    BALISE_FIN_DOCUMENT,     // Balise indiquant la fin d'un document.
    BALISE_ANNEXE,           // Balise indiquant le début d'une annexe.
    BALISE_FIN_ANNEXE,       // Balise indiquant la fin d'une annexe.
    BALISE_TITRE,            // Balise indiquant le début d'un titre.
    BALISE_FIN_TITRE,        // Balise indiquant la fin d'un titre.
    BALISE_LISTE,            // Balise indiquant le début d'une liste.
    BALISE_FIN_LISTE,        // Balise indiquant la fin d'une liste.
    BALISE_MOT_IMPORTANT,    // Balise indiquant le début d'un mot important.
    BALISE_FIN_MOT_IMPORTANT,// Balise indiquant la fin d'un mot important.
    BALISE_SECTION,          // Balise indiquant le début d'une section.
    BALISE_FIN_SECTION,      // Balise indiquant la fin d'une section.
    BALISE_ITEM,             // Balise indiquant le début d'un item de liste.
    BALISE_FIN_ITEM,         // Balise indiquant la fin d'un item de liste.
    BALISE_RETOUR_LIGNE,     // Balise pour un retour à la ligne.
    BALISE_TEXTE             // Balise pour du texte simple.
} balise_t;

/**
 * Structure représentant un nœud de l'arbre syntaxique.
 * Chaque nœud peut contenir une balise, du contenu textuel, et des références vers des nœuds enfants et un parent.
 */
typedef struct Node
{
    balise_t balise;         // Type de balise du nœud.
    char *contenu;           // Contenu textuel du nœud, le cas échéant.
    int nb_enfants;          // Nombre d'enfants du nœud.
    struct Node **enfants;   // Pointeur vers les enfants du nœud.
    struct Node *parent;     // Pointeur vers le nœud parent.
    int profondeur;          // Profondeur du nœud dans l'arbre.
} Node;

/**
 * Structure représentant l'état global de l'analyseur lors du traitement d'un document nanoml.
 * Contient les références aux fichiers d'entrée et de sortie, ainsi que les nœuds courants de l'arbre syntaxique.
 */
typedef struct
{
    FILE *fichier;           // Fichier source en cours de lecture.
    int charac;              // Dernier caractère lu du fichier.
    balise_t balise_actuelle;// Balise actuellement traitée par l'analyseur.
    FILE *output;            // Fichier de sortie pour l'écriture du document structuré.
    Node *noeud_courant;     // Nœud courant de l'arbre en cours de traitement.
    Node *noeud_racine;      // Nœud racine de l'arbre syntaxique.
} element;



#endif
