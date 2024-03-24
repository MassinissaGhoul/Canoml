#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum
{
    DEBUT_DOC,
    FIN_DOC,
    DEBUT_ANNEXE,
    FIN_ANNEXE,
    DEBUT_SECTION,
    FIN_SECTION,
    DEBUT_TITRE,
    FIN_TITRE,
    DEBUT_LISTE,
    FIN_LISTE,
    DEBUT_ITEM,
    FIN_ITEM,
    DEBUT_IMPORTANT,
    FIN_IMPORTANT,
    MOT_SIMPLE,
    RETOUR_LIGNE,
    FINITO
} type_balise;

typedef struct
{
    type_balise type;
    char valeur[81];
} balise;

typedef struct noeud_pile {
    balise b;
    struct noeud_pile *suivant;
} noeud_pile;

typedef noeud_pile *pile;

void initialiser_pile(pile *p)
{
    *p = NULL;
}

void empiler(pile *p, balise b)
{
    noeud_pile *nouveau = malloc(sizeof(noeud_pile));
    nouveau->b = b;
    nouveau->suivant = *p;
    *p = nouveau;
}

balise depiler(pile *p)
{
    balise b;
    noeud_pile *temp;

    if (*p == NULL)
    {
        b.type = FINITO;
        return b;
    }

    temp = *p;
    b = temp->b;
    *p = temp->suivant;
    free(temp);

    return b;
}

int pile_vide(pile p)
{
    return p == NULL;
}

balise readBalise(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char c;
    int i = 0;
    char *tag[14] = {"<document>", "</document>", "<annexe>", "</annexe>", "<section>", "</section>", "<titre>", "</titre>", "<liste>", "</liste>", "<item>", "</item>", "<important>", "</important>"};
    do
    {
        c = fgetc(fichier_entree);
    } while (c == ' ' || c == '\n');

    if (c == EOF)
    {
        b.type = FINITO;
        return b;
    }

    if (c == '<')
    {
        b.valeur[i++] = c;
        do
        {
            c = fgetc(fichier_entree);
            b.valeur[i++] = c;
        } while (c != '>' && c != EOF);
        b.valeur[i] = '\0';
        for (int j = 0; j < 14; j++)
        {
            if (strcmp(b.valeur, tag[j]) == 0)
            {
                if (j % 2 == 0)
                {
                    fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", tag[j]);
                    b.type = j;
                }
                else
                {
                    fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", tag[j]);
                    b.type = j;
                }
            }
        }
    }
    else
    {
        i = 0;
        do
        {
            b.valeur[i++] = c;
            c = fgetc(fichier_entree);
        } while (c != ' ' && c != '\n' && c != '<' && c != EOF);
        b.valeur[i] = '\0';
        if (strcmp(b.valeur, "retour_a_la_ligne") == 0)
        {
            b.type = RETOUR_LIGNE;
        }
        else
        {
            fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);
            b.type = MOT_SIMPLE;
        }
    }

    return b;
}

int analyse_syntaxique(FILE *fichier_entree, FILE *fichier_sortie)
{
    pile p;
    balise b, b_fermante;
    int correct = 1;

    initialiser_pile(&p);

    while (correct && (b = readBalise(fichier_entree, fichier_sortie)).type != FINITO)
    {
        switch (b.type)
        {
            case DEBUT_DOC:
            case DEBUT_ANNEXE:
            case DEBUT_SECTION:
            case DEBUT_TITRE:
            case DEBUT_LISTE:
            case DEBUT_ITEM:
            case DEBUT_IMPORTANT:
                empiler(&p, b);
                break;
            case FIN_DOC:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_DOC)
                {
                    correct = 0;
                }
                break;
            case FIN_ANNEXE:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_ANNEXE)
                {
                    correct = 0;
                }
                break;
            case FIN_SECTION:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_SECTION)
                {
                    correct = 0;
                }
                break;
            case FIN_TITRE:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_TITRE)
                {
                    correct = 0;
                }
                break;
            case FIN_LISTE:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_LISTE)
                {
                    correct = 0;
                }
                break;
            case FIN_ITEM:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_ITEM)
                {
                    correct = 0;
                }
                break;
            case FIN_IMPORTANT:
                b_fermante = depiler(&p);
                if (b_fermante.type != DEBUT_IMPORTANT)
                {
                    correct = 0;
                }
                break;
            default:
                break;
        }
    }

    if (!pile_vide(p))
    {
        correct = 0;
    }

    if (correct)
    {
        fprintf(fichier_sortie, "L'analyse syntaxique est correcte.\n");
    }
    else
    {
        fprintf(fichier_sortie, "Il y a une erreur syntaxique dans le fichier.\n");
    }

    return correct;
}

int main()
{
    FILE *fichier_entree = fopen("input.txt", "r");
    FILE *fichier_sortie = fopen("output.txt", "w");
    if (fichier_entree == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }
    if (fichier_sortie == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return 1;
    }

    if (analyse_syntaxique(fichier_entree, fichier_sortie))
    {
        fprintf(fichier_sortie, "L'analyse syntaxique est réussie\n");
    }
    else
    {
        fprintf(fichier_sortie, "Il y a une erreur syntaxique dans le fichier\n");
    }

    fclose(fichier_entree);
    fclose(fichier_sortie);

    return 0;
}



