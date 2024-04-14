#ifndef ANALYSEUR_H
#define ANALYSEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"


void ouverture(element *e, char *nom_fichier);
void fermeture(element *e);
void lire_caractere(element *e);
void passer_espace(element *e);
void update_balise_actuelle(element *e, char *token);
void lire_token(element *e);
void consommer(element *e, char caractere);
void consommer_token(element *e, char *token);
void texte_enrichi(element *e);
void document(element *e);
void contenu(element *e, int balise_contenante);
void section(element *e);
void titre(element *e);
void texte(element *e);
void liste(element *e);
void item(element *e);
void annexes(element *e);
void mot_enrichi(element *e);
void mot_important(element *e);
void mot_simple(element *e);
void fermeture_balise(element *e, int balise_contenante);
void afficher_arbre(Node *noeud, int profondeur, FILE *fichier);
void printNodeData(Node *noeud, int level, Node *parent);




#endif