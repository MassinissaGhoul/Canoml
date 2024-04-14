#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "analyseur.h"
#include "arbre.h"


Node *creer_noeud(balise_t balise, char *contenu)
{
    Node *noeud = malloc(sizeof(Node));
    noeud->balise = balise;
    noeud->contenu = contenu;
    noeud->nb_enfants = 0;
    noeud->enfants = NULL;
    noeud->parent = NULL;
    noeud->profondeur = 0; 

    return noeud;
}


void ajouter_enfant(Node *parent, Node *enfant)
{
    parent->nb_enfants++;
    parent->enfants = realloc(parent->enfants, sizeof(Node *) * parent->nb_enfants);
    parent->enfants[parent->nb_enfants - 1] = enfant;
    enfant->parent = parent;
    enfant->profondeur = parent->profondeur + 1; 
}


void ecrire_arbre(FILE *output, Node *noeud)
{
    if (noeud == NULL)
    {
        return;
    }
    switch (noeud->balise)
    {
    case BALISE_DOCUMENT:
        fprintf(output, "<document>\n");
        break;
    case BALISE_FIN_DOCUMENT:
        fprintf(output, "</document>\n");
        break;
    case BALISE_ANNEXE:
        fprintf(output, "<annexe>\n");
        break;
    case BALISE_FIN_ANNEXE:
        fprintf(output, "</annexe>\n");
        break;
    case BALISE_TITRE:
        fprintf(output, "<titre>\n");
        break;
    case BALISE_FIN_TITRE:
        fprintf(output, "</titre>\n");
        break;
    case BALISE_LISTE:
        fprintf(output, "<liste>\n");
        break;
    case BALISE_FIN_LISTE:
        fprintf(output, "</liste>\n");
        break;
    case BALISE_MOT_IMPORTANT:
        fprintf(output, "<mot_important>%s</mot_important>\n", noeud->contenu);
        break;
    case BALISE_SECTION:
        fprintf(output, "<section>\n");
        break;
    case BALISE_FIN_SECTION:
        fprintf(output, "</section>\n");
        break;
    case BALISE_ITEM:
        fprintf(output, "<item>\n");
        break;
    case BALISE_FIN_ITEM:
        fprintf(output, "</item>\n");
        break;
    case BALISE_RETOUR_LIGNE:
        fprintf(output, "<br/>\n");
        break;
    case BALISE_TEXTE:
        fprintf(output, "%s\n", noeud->contenu);
        break;
    default:
        fprintf(output, "balise inconnue\n");
        break;
    }
    for (int i = 0; i < noeud->nb_enfants; i++)
    {
        ecrire_arbre(output, noeud->enfants[i]);
    }
}


void afficher_arbre(Node *noeud, int profondeur, FILE *fichier)
{
    if (noeud == NULL)
    {
        return;
    }
    for (int i = 0; i < profondeur; i++)
    {
        fprintf(fichier, "  ");
    }
    switch (noeud->balise)
    {
    case BALISE_DOCUMENT:
        fprintf(fichier, "<document>\n");
        break;
    case BALISE_FIN_DOCUMENT:
        fprintf(fichier, "</document>\n");
        break;
    case BALISE_ANNEXE:
        fprintf(fichier, "<annexe>\n");
        break;
    case BALISE_FIN_ANNEXE:
        fprintf(fichier, "</annexe>\n");
        break;
    case BALISE_TITRE:
        fprintf(fichier, "<titre>\n");
        break;
    case BALISE_FIN_TITRE:
        fprintf(fichier, "</titre>\n");
        break;
    case BALISE_LISTE:
        fprintf(fichier, "<liste>\n");
        break;
    case BALISE_FIN_LISTE:
        fprintf(fichier, "</liste>\n");
        break;
    case BALISE_MOT_IMPORTANT:
        fprintf(fichier, "<mot_important>%s</mot_important>\n", noeud->contenu);
        break;
    case BALISE_SECTION:
        fprintf(fichier, "<section>\n");
        break;
    case BALISE_FIN_SECTION:
        fprintf(fichier, "</section>\n");
        break;
    case BALISE_ITEM:
        fprintf(fichier, "<item>\n");
        break;
    case BALISE_FIN_ITEM:
        fprintf(fichier, "</item>\n");
        break;
    case BALISE_RETOUR_LIGNE:
        fprintf(fichier, "<br/>\n");
        break;
    case BALISE_TEXTE:
        fprintf(fichier, "%s\n", noeud->contenu);
        break;
    default:
        fprintf(fichier, "balise inconnue\n");
        break;
    }
    for (int i = 0; i < noeud->nb_enfants; i++)
    {
        afficher_arbre(noeud->enfants[i], profondeur + 1, fichier);
    }
}


void printNodeData(Node *noeud, int level, Node *parent)
{
    int increment = 1;
    if (noeud->balise % 2 == 0)
    {
        increment = -1;
    }

    for (int i = 0; i < noeud->profondeur; ++i)
    {
        printf("  ");
    }

    if (noeud->contenu != NULL)
    {
        printf("Niveau %d, Balise: %d, Contenu: %s, Parent: ", noeud->profondeur, noeud->balise, noeud->contenu);
        if (parent != NULL)
        {
            printf("Niveau %d, Balise: %d, Contenu: %s\n", noeud->profondeur, parent->balise, parent->contenu);
        }
        else
        {
            printf("Racine\n");
        }
    }
    else
    {
        printf("Niveau %d, Balise: %d, Contenu: Vide, Parent: ", noeud->profondeur, noeud->balise);
        if (parent != NULL)
        {
            printf("Niveau %d, Balise: %d, Contenu: %s\n", noeud->profondeur, parent->balise, parent->contenu);
        }
        else
        {
            printf("Racine\n");
        }
    }

    level += increment;

    for (int i = 0; i < noeud->nb_enfants; ++i)
    {
        printNodeData(noeud->enfants[i], level, noeud);
    }
}


void printFormattedLine(FILE *fichier, const char *text, int width, int depth, const char *prefix) {
    fprintf(fichier, "|");
    if (prefix) {
        fprintf(fichier, "%s ", prefix);
    }
    fprintf(fichier, "%s", text);
    int used = strlen(text) + (prefix ? strlen(prefix) + 1 : 0);
    for (int i = used; i < width - 3; i++) {
        fprintf(fichier, " ");
    }
    fprintf(fichier, "|\n");
}


void printBox(FILE *fichier, int width) {
    fprintf(fichier, "+");
    for (int i = 0; i < width - 2; i++) {
        fprintf(fichier, "-");
    }
    fprintf(fichier, "+\n");
}


void afficher_arbre_box(Node *noeud, FILE *fichier, int depth) {
    if (!noeud) return;

    if (noeud->balise == BALISE_DOCUMENT || noeud->balise == BALISE_SECTION || noeud->balise == BALISE_LISTE) {
        printBox(fichier, 50);
    }

    switch (noeud->balise) {
    case BALISE_TITRE:
        printFormattedLine(fichier, noeud->contenu, 50, depth, NULL);
        break;
    case BALISE_ITEM:
        printFormattedLine(fichier, noeud->contenu, 50, depth, "#");
        break;
    case BALISE_TEXTE:
        printFormattedLine(fichier, noeud->contenu, 50, depth, NULL);
        break;
    default:
        break;
    }

    for (int i = 0; i < noeud->nb_enfants; i++) {
        afficher_arbre_box(noeud->enfants[i], fichier, depth + 1);
    }

    if (noeud->balise == BALISE_DOCUMENT || noeud->balise == BALISE_SECTION || noeud->balise == BALISE_LISTE) {
        printBox(fichier, 50);
    }
}


void liberer_noeud(Node *noeud) {
    if (noeud == NULL) {
        return;
    }
    free(noeud->contenu);  // Libération du contenu du nœud
    noeud->contenu = NULL;
    for (int i = 0; i < noeud->nb_enfants; i++) {
        liberer_noeud(noeud->enfants[i]);  // Libération récursive des enfants
    }
    free(noeud->enfants);  // Libération du tableau des enfants
    noeud->enfants = NULL;
    free(noeud);  // Libération de la structure du nœud lui-même
}


void liberer_arbre(Node *racine) {
    liberer_noeud(racine);  // Commence par la racine et libère tout récursivement
}