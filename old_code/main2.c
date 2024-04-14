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
    type_balise type; // 0
    char valeur[81]; // <document>
} balise;

balise lire_mot(FILE *fichier_entree, char *buffer, int taille_buffer);

int analyse_syntaxique(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_document(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_annexe(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_section(FILE *fichier_entree, FILE *fichier_sortie, balise b);
int analyse_titre(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_liste(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_item(FILE *fichier_entree, FILE *fichier_sortie);
int analyse_important(FILE *fichier_entree, FILE *fichier_sortie);
void skip_spaces(FILE *fichier_entree);
void writeBalise(FILE* fichier_entree, FILE* fichier_sortie, balise b);
int detectTag(FILE* fichier_entree, FILE* fichier_sortie, balise b);





// int analyse_annexes(FILE *fichier_entree, FILE *fichier_sortie);

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

void skip_spaces(FILE *fichier_entree)
{
    int c;
    do
    {
        c = fgetc(fichier_entree);
    } while (c == ' ' || c == '\n' || c == '\t' || c == '\r');
    ungetc(c, fichier_entree);
}

balise lire_mot(FILE *fichier_entree, char *buffer, int taille_buffer)
{
    balise b;
    char c;
    int i = 0;
    char *tag[14] = {"<document>", "</document>", "<annexe>", "</annexe>", "<section>", "</section>", "<titre>", "</titre>", "<liste>", "</liste>", "<item>", "</item>", "<important>", "</important>"};

    do
    {
        skip_spaces(fichier_entree);

        c = fgetc(fichier_entree);
    } while (c == ' ' || c == '\n' || c == '\t' || c == '\r');

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
                b.type = j;
                break;
            }
        }
    }
    else
    {
        i = 0;
        do
        {
            buffer[i++] = c;
            c = fgetc(fichier_entree);
            if (c == ' ')
            {
                buffer[i] = '\0';
                strcpy(b.valeur, buffer);
                b.type = MOT_SIMPLE;
                ungetc(c, fichier_entree);
                return b;
            }
        } while (c != '<' && c != EOF);
        buffer[i] = '\0';
        if (strcmp(buffer, "retour_a_la_ligne") == 0)
        {
            b.type = RETOUR_LIGNE;
        }
        else
        {
            b.type = MOT_SIMPLE;
            strcpy(b.valeur, buffer);
        }
        ungetc(c, fichier_entree);
    }

    return b;
}

int analyse_syntaxique(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];
    int correct = 1;

    if (!analyse_document(fichier_entree, fichier_sortie))
    {
        correct = 0;
    }

    if (!analyse_annexe(fichier_entree, fichier_sortie))
    {
        correct = 0;
    }

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != FINITO)
    {
        correct = 0;
    }

    if (!correct)
    {
        fprintf(fichier_sortie, "Il y a une erreur syntaxique dans le fichier.\n");
    }
    else
    {
        fprintf(fichier_sortie, "L'analyse syntaxique est correcte.\n");
    }

    return correct;
}



int analyse_document(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != DEBUT_DOC)
    {
        return 0;
    }
    fprintf(fichier_sortie, "(balise_ouvrante, \"<document>\")\n");

    while (1)
    {
        b = lire_mot(fichier_entree, buffer, sizeof(buffer));
        printf("b.valeur %s\n", b.valeur);
        if (b.type == FIN_DOC)
        {
            break;
        }
        else if (b.type == DEBUT_SECTION)
        {
            if (!analyse_section(fichier_entree, fichier_sortie, b))
            {
                return 0;
            }
        }
        else if (b.type == DEBUT_ANNEXE)
        {
            if (!analyse_annexe(fichier_entree, fichier_sortie))
            {
                return 0;
            }
        }
        else if (b.type == DEBUT_TITRE)
        {
            if (!analyse_titre(fichier_entree, fichier_sortie))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</document>\")\n");
    return 1;
}


// int analyse_annexes(FILE *fichier_entree, FILE *fichier_sortie)
// {
//     balise b;
//     char buffer[81];

//     fprintf(fichier_sortie, "(balise_ouvrante, \"<annexes>\")\n");

//     while (1)
//     {
//         b = lire_mot(fichier_entree, buffer, sizeof(buffer));

//         if (b.type == FINITO || b.type == FIN_ANNEXE)
//         {
//             break;
//         }
//         else if (b.type == DEBUT_ANNEXE)
//         {
//             if (!analyse_annexe(fichier_entree, fichier_sortie))
//             {
//                 return 0;
//             }
//         }
//         else
//         {
//             return 1;
//         }
//     }

//     fprintf(fichier_sortie, "(balise_fermante, \"</annexes>\")\n");

//     return 1;
// }

int analyse_annexe(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != DEBUT_ANNEXE)
    {
        return 0;
    }
    fprintf(fichier_sortie, "(balise_ouvrante, \"<annexe>\")\n");

    while (1)
    {
        b = lire_mot(fichier_entree, buffer, sizeof(buffer));

        if (b.type == FIN_ANNEXE)
        {
            break;
        }
        else if (b.type == MOT_SIMPLE)
        {
            fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);
        }
        else
        {
            return 0;
        }
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</annexe>\")\n");
    return 1;
}

int analyse_section(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];
    fprintf(fichier_sortie, "(balise_ouvrante, \"<section>\")\n");

    while (1)
    {
        b = lire_mot(fichier_entree, buffer, sizeof(buffer));
        printf("b.valeur = %s", b.valeur);
        if (b.type == FIN_SECTION)
        {
            break;
        }
        else if (b.type == MOT_SIMPLE)
        {
            fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);
        }
        else if (b.type == DEBUT_TITRE)
        {
            if (!analyse_titre(fichier_entree, fichier_sortie))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</section>\")\n");
    return 1;
}

// int analyse_section(FILE *fichier_entree, FILE *fichier_sortie, balise b){
//     writeBalise(fichier_entree, fichier_sortie, b);
//     b = lire_mot(fichier_entree, b.valeur, sizeof(b.valeur));
//     detectTag(fichier_entree, fichier_sortie, b);

// }

int analyse_titre(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];

    fprintf(fichier_sortie, "(balise_ouvrante, \"<titre>\")\n");


    while(1)
    {
        b = lire_mot(fichier_entree, buffer, sizeof(buffer));


        if (b.type == FIN_TITRE)
        {
            break;
        }
        else if (b.type == MOT_SIMPLE)
        {
            fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);

        }
        else
        {
            return 0;
        }
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</titre>\")\n");
    return 1;
}

int analyse_liste(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];

    fprintf(fichier_sortie, "(balise_ouvrante, \"<liste>\")\n");

    while (1)
    {
        b = lire_mot(fichier_entree, buffer, sizeof(buffer));

        if (b.type == FIN_LISTE)
        {
            break;
        }
        else if (b.type == DEBUT_ITEM)
        {
            if (!analyse_item(fichier_entree, fichier_sortie))
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</liste>\")\n");
    return 1;
}

int analyse_item(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];


    fprintf(fichier_sortie, "(balise_ouvrante, \"<item>\")\n");

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != MOT_SIMPLE)
    {
        return 0;
    }
    fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != FIN_ITEM)
    {
        return 0;
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</item>\")\n");
    return 1;
}

int analyse_important(FILE *fichier_entree, FILE *fichier_sortie)
{
    balise b;
    char buffer[81];


    fprintf(fichier_sortie, "(balise_ouvrante, \"<important>\")\n");

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != MOT_SIMPLE)
    {
        return 0;
    }
    fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);

    b = lire_mot(fichier_entree, buffer, sizeof(buffer));

    if (b.type != FIN_IMPORTANT)
    {
        return 0;
    }

    fprintf(fichier_sortie, "(balise_fermante, \"</important>\")\n");
    return 1;
}



int detectTag(FILE* fichier_entree, FILE* fichier_sortie, balise b){

    switch(b.type){
        case DEBUT_DOC:
        case FIN_DOC:
            analyse_document(fichier_entree, fichier_sortie);
            break;
        case DEBUT_SECTION:
        case FIN_SECTION:
            analyse_section(fichier_entree, fichier_sortie, b);
            break;
        case DEBUT_ANNEXE:
        case FIN_ANNEXE:
            analyse_annexe(fichier_entree, fichier_sortie);
            break;
        case DEBUT_TITRE:
        case FIN_TITRE:
            analyse_titre(fichier_entree, fichier_sortie);
            break;
        case DEBUT_LISTE:
        case FIN_LISTE:
            analyse_liste(fichier_entree, fichier_sortie);
            break;
        case DEBUT_ITEM:
        case FIN_ITEM:
            analyse_item(fichier_entree, fichier_sortie);
            break;
        case DEBUT_IMPORTANT:
        case FIN_IMPORTANT:
            analyse_important(fichier_entree, fichier_sortie);
            break;
        case MOT_SIMPLE:
            fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);
            break;
        
    }
}

void writeBalise(FILE* fichier_entree, FILE* fichier_sortie, balise b)
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
        fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", b.valeur);
        break;
    case FIN_DOC:
    case FIN_ANNEXE:
    case FIN_SECTION:
    case FIN_TITRE:
    case FIN_LISTE:
    case FIN_ITEM:
    case FIN_IMPORTANT:
        fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", b.valeur);
        break;    
    }
}

