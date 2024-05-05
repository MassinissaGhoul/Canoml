#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "analyseur.h"
#include "arbre.h"


void ouverture(element *e, char *nom_fichier)
{
    e->fichier = fopen(nom_fichier, "r");
    if (e->fichier == NULL)
    {
        exit(-1);
    }
    e->output = fopen("output.txt", "w");
    lire_caractere(e);
}


void fermeture(element *e)
{
    ecrire_arbre(e->output, e->noeud_racine);
    fclose(e->fichier);
    fclose(e->output);
}


void lire_caractere(element *e)
{
    e->charac = fgetc(e->fichier);
}


void passer_espace(element *e)
{
    while (e->charac == ' ' || e->charac == '\t' || e->charac == '\n' || e->charac == '\r')
    {
        lire_caractere(e);
    }
}


balise_t get_balise(const char *token)
{
    static const char *balises[] = {
        "<document>",
        "</document>",
        "<annexe>",
        "</annexe>",
        "<titre>",
        "</titre>",
        "<liste>",
        "</liste>",
        "<mot_important>",
        "</mot_important>",
        "<section>",
        "</section>",
        "<item>",
        "</item>",
        "<br/>"};

    for (int i = 0; i < 15; ++i)
    {
        if (strcmp(token, balises[i]) == 0)
        {
            return i + 1;
        }
    }

    return BALISE_INCONNUE;
}


void update_balise_actuelle(element *e, char *token)
{
    balise_t balise = get_balise(token);

    switch (balise)
    {
    case BALISE_DOCUMENT:
        e->balise_actuelle = BALISE_DOCUMENT;
        
        break;
    case BALISE_FIN_DOCUMENT:
        e->balise_actuelle = BALISE_FIN_DOCUMENT;
        break;
    case BALISE_ANNEXE:
        e->balise_actuelle = BALISE_ANNEXE;
        break;
    case BALISE_FIN_ANNEXE:
        e->balise_actuelle = BALISE_FIN_ANNEXE;
        break;
    case BALISE_TITRE:
        e->balise_actuelle = BALISE_TITRE;
        break;
    case BALISE_FIN_TITRE:
        e->balise_actuelle = BALISE_FIN_TITRE;
        break;
    case BALISE_LISTE:
        e->balise_actuelle = BALISE_LISTE;
        break;
    case BALISE_FIN_LISTE:
        e->balise_actuelle = BALISE_FIN_LISTE;
        break;
    case BALISE_MOT_IMPORTANT:
        e->balise_actuelle = BALISE_MOT_IMPORTANT;
        break;
    case BALISE_FIN_MOT_IMPORTANT:
        e->balise_actuelle = BALISE_FIN_MOT_IMPORTANT;
        break;
    case BALISE_SECTION:
        e->balise_actuelle = BALISE_SECTION;
        break;
    case BALISE_FIN_SECTION:
        e->balise_actuelle = BALISE_FIN_SECTION;
        break;
    case BALISE_ITEM:
        e->balise_actuelle = BALISE_ITEM;
        break;
    case BALISE_FIN_ITEM:
        e->balise_actuelle = BALISE_FIN_ITEM;
        break;
    case BALISE_RETOUR_LIGNE:
        e->balise_actuelle = BALISE_RETOUR_LIGNE;
        break;
    default:
        printf("\nERREUR :balise non existante: %s . \n", token);
        e->balise_actuelle = BALISE_INCONNUE;
        exit(-1);
    }
}


void lire_token(element *e)
{
    char token[20];
    int i = 0;

    while (e->charac != '>' && e->charac != EOF)
    {
        token[i] = e->charac;
        lire_caractere(e);
        i++;
    }

    if (e->charac == '>')
    {
        token[i] = '>';
        token[i + 1] = '\0';
        consommer(e, '>');
        update_balise_actuelle(e, token);
        // fprintf(e->output, "%s", token);
        char *contenu = strdup(token);

        Node *noeud = creer_noeud(get_balise(token),contenu);
        ajouter_enfant(e->noeud_courant, noeud);
        e->noeud_courant = noeud;
    }
    else
    {
        printf("Erreur: caractère '>' non trouvé\n");
        exit(-1);
    }
}


void consommer(element *e, char caractere)
{
    if (e->charac == caractere)
    {   
        lire_caractere(e);
    }
    else
    {   
        printf("Erreur: caractere attendu: %c, caractere trouve: %c\n", caractere, e->charac);
    }
}


void consommer_token(element *e, char *token)
{
    int i = 0;
    while (token[i] != '\0')
    {
        consommer(e, token[i]);
        i++;
    }
    update_balise_actuelle(e, token);
}


void texte_enrichi(element *e)
{
    document(e);
    while (e->charac != EOF)
    {
        if (e->charac == '<')
        {
            lire_token(e);
            if (e->balise_actuelle == BALISE_ANNEXE)
            {
                annexes(e);
            }
            else
            {
                printf("Erreur: balise non reconnue ou incorrecte\n");
                exit(-1);
            }
        }
        else if (e->charac != '\n' && e->charac != ' ')
        {
            mot_simple(e);
        }
    }
}


void document(element *e)
{
    Node *noeud = creer_noeud(BALISE_DOCUMENT, " <document>");
    e->noeud_racine = noeud;
    e->noeud_courant = noeud;
    consommer_token(e, "<document>");
    passer_espace(e);
    contenu(e, BALISE_FIN_DOCUMENT);
    passer_espace(e);
}


void contenu(element *e, int balise_contenante)
{
    while ((int)e->balise_actuelle != balise_contenante)
    {
        if (e->charac == EOF)
        {
            printf("Erreur: balise de fin manquante\n");
            exit(-1);
        }
        if (e->charac == '<')
        {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e);
            switch (e->balise_actuelle)
            {
            case BALISE_FIN_DOCUMENT:
            case BALISE_FIN_ANNEXE:
            case BALISE_FIN_SECTION:
            case BALISE_FIN_TITRE:
            case BALISE_FIN_LISTE:
            case BALISE_FIN_ITEM:
                fermeture_balise(e, balise_contenante);
                break;
            case BALISE_SECTION:
                section(e);
                break;
            case BALISE_TITRE:
                titre(e);
                break;
            case BALISE_LISTE:
                liste(e);
                passer_espace(e);
                break;
            case BALISE_MOT_IMPORTANT:
                mot_enrichi(e);
                break;
            case BALISE_FIN_MOT_IMPORTANT:
                break;
            case BALISE_RETOUR_LIGNE:
                mot_enrichi(e);
                e->balise_actuelle = balise_ancienne;
                break;
            default:
                printf("Erreur: balise non reconnue ou incorrecte\n");
                exit(-1);
                break;
            }
        }
        else
        {
            mot_enrichi(e);
        }
    }
    passer_espace(e);
}


void section(element *e)
{
    passer_espace(e);
    contenu(e, BALISE_FIN_SECTION);
    passer_espace(e);
}


void titre(element *e)
{
    passer_espace(e);
    texte(e);
    passer_espace(e);
}


void texte(element *e)
{
    while (e->balise_actuelle != BALISE_FIN_TITRE)
    {
        if (e->charac == '<')
        {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e);
            switch (e->balise_actuelle)
            {
            case BALISE_FIN_TITRE:
                fermeture_balise(e, BALISE_FIN_TITRE);
                break;
            case BALISE_MOT_IMPORTANT:
                mot_enrichi(e);
                break;
            case BALISE_RETOUR_LIGNE:
                mot_enrichi(e);
                e->balise_actuelle = balise_ancienne;
                break;
            default:
                printf("Erreur: balise non reconnue ou incorrecte\n");
                exit(-1);
                break;
            }
        }
        else
        {
            mot_enrichi(e);
        }
    }
}


void liste(element *e)
{
    while (e->balise_actuelle != BALISE_FIN_LISTE)
    {
        passer_espace(e);
        if (e->charac == '<')
        {
            lire_token(e);
            if (e->balise_actuelle == BALISE_FIN_LISTE)
            {
                fermeture_balise(e, BALISE_FIN_LISTE);
                break;
            }
            else if (e->balise_actuelle == BALISE_ITEM)
            {
                item(e);
                passer_espace(e);
            }
            else
            {
                printf("Erreur 1: balise liste non fermée\n");
                exit(-1);
            }
        }
    }
}


void item(element *e)
{
    while (e->balise_actuelle != BALISE_FIN_ITEM)
    {
        if (e->charac == '<')
        {
            lire_token(e);
            if (e->balise_actuelle == BALISE_FIN_ITEM)
            {
                fermeture_balise(e, BALISE_FIN_ITEM);
                break;
            }
            else if (e->balise_actuelle == BALISE_LISTE)
            {
                liste(e);
            }
            else if (e->balise_actuelle == BALISE_FIN_LISTE)
            {
                break;
            }
            else
            {
                printf("Erreur 2: balise item non fermée\n");
                exit(-1);
            }
        }
        else
        {
            mot_enrichi(e);
        }
    }
}


void annexes(element *e)
{
    consommer_token(e, "<annexe>");
    passer_espace(e);
    contenu(e, BALISE_FIN_ANNEXE);
    passer_espace(e);
}


void mot_enrichi(element *e)
{
    if (e->balise_actuelle == BALISE_MOT_IMPORTANT)
    {
        passer_espace(e);
        mot_important(e);
        passer_espace(e);
    }
    else if (e->balise_actuelle == BALISE_RETOUR_LIGNE)
    {
        passer_espace(e);
    }
    else
    {
        mot_simple(e);
    }
}


void mot_important(element *e)
{
    while (e->balise_actuelle != BALISE_FIN_MOT_IMPORTANT)
    {
        if (e->charac == '<')
        {
            lire_token(e);
            if (e->balise_actuelle == BALISE_FIN_MOT_IMPORTANT)
            {
                fermeture_balise(e, BALISE_FIN_MOT_IMPORTANT);
                break;
            }
            else
            {
                printf("Erreur: balise important non fermée\n");
                exit(-1);
            }
        }
        else
        {
            mot_simple(e);
        }
    }
}


void mot_simple(element *e)
{
    char mot[100];
    int i = 0;
    while (e->charac != ' ' && e->charac != '\n' && e->charac != '<' && e->charac != EOF && i < 99)
    {
        mot[i] = e->charac;
        lire_caractere(e);
        i++;
    }
    mot[i] = '\0';
    char *contenu = strdup(mot);
    if (!contenu)
    {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    printf("mot = %s\n", contenu);

    Node *noeud = creer_noeud(BALISE_TEXTE, contenu);
    ajouter_enfant(e->noeud_courant, noeud);
    // fprintf(e->output, "%s ", contenu);
    passer_espace(e);
}


void fermeture_balise(element *e, int balise_contenante)
{
    while ((int)e->noeud_courant->balise != balise_contenante)
    {
        if (e->noeud_courant->parent == NULL)
        {
            printf("Erreur: balise de fin manquante pour %d\n", balise_contenante);
            exit(-1);
        }
        e->noeud_courant = e->noeud_courant->parent;
    }
    if (e->noeud_courant->parent != NULL)
    {
        e->noeud_courant = e->noeud_courant->parent; // Remonter au parent
    }
    else
    {
        printf("Avertissement: la balise de fin pour %d est à la racine.\n", balise_contenante);
    }
    e->noeud_courant->profondeur--;
}