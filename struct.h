#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum
{
    BALISE_INCONNUE,
    BALISE_DOCUMENT,
    BALISE_FIN_DOCUMENT,
    BALISE_ANNEXE,
    BALISE_FIN_ANNEXE,
    BALISE_TITRE,
    BALISE_FIN_TITRE,
    BALISE_LISTE,
    BALISE_FIN_LISTE,
    BALISE_MOT_IMPORTANT,
    BALISE_FIN_MOT_IMPORTANT,
    BALISE_SECTION,
    BALISE_FIN_SECTION,
    BALISE_ITEM,
    BALISE_FIN_ITEM,
    BALISE_RETOUR_LIGNE,
    BALISE_TEXTE 
} balise_t;


typedef struct Node
{
    balise_t balise;
    char *contenu;
    int nb_enfants;
    struct Node **enfants;
    struct Node *parent;
    int profondeur; 
} Node;


typedef struct
{
    FILE *fichier;
    int charac;
    balise_t balise_actuelle;
    FILE *output;
    Node *noeud_courant;
    Node *noeud_racine;
} element;




#endif