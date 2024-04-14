#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"



Node *creer_noeud(balise_t balise, char *contenu);
void ajouter_enfant(Node *parent, Node *enfant);
void ecrire_arbre(FILE *output, Node *noeud);

void afficher_arbre_box(Node *noeud, FILE *fichier, int depth);
void printFormattedLine(FILE *fichier, const char *text, int width, int depth, const char *prefix);
void printBox(FILE *fichier, int width);
void liberer_noeud(Node *noeud);
void liberer_arbre(Node *racine);


#endif