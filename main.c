#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "analyseur.h"
#include "arbre.h"



int main()
{
    element e;
    e.noeud_courant = NULL;
    e.noeud_racine = NULL;
    ouverture(&e, "input.txt");
    texte_enrichi(&e);
    fermeture(&e);
    FILE *fichier = fopen("arbre.txt", "w");
    FILE *fichier2 = fopen("bloc.txt", "w");
    // afficher_arbre(e.noeud_racine, fichier, 0, 1);

    
    afficher_arbre(e.noeud_racine, 0, fichier);
    printNodeData(e.noeud_racine, 0, NULL);    // printBox(e.noeud_racine, 0, fichier2);
    afficher_arbre_box(e.noeud_racine, fichier2, 0);
    
    // arbre_vers_affichage(e.noeud_racine, 80, fichier2);


    fclose(fichier2);

    fclose(fichier);
    liberer_arbre(e.noeud_racine); 
    return 0;
}