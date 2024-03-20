#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
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
    RETOUR_LIGNE
} type_balise;

typedef struct {
    type_balise type;
    char valeur[81];
} balise;

balise lire_prochaine_balise(FILE *fichier_entree, FILE *fichier_sortie) {
    balise b;
    char c;
    int i = 0;

    do {
        c = fgetc(fichier_entree);
    } while (c == ' ' || c == '\n');

    if (c == EOF) {
        return (balise) {0, ""};
    }

    while (c != EOF) {
        do {
            c = fgetc(fichier_entree);
        } while (c == ' ' || c == '\n');

        if (c == '<') {
            b.valeur[i++] = c; 

            do {
                c = fgetc(fichier_entree);
                b.valeur[i++] = c;
            } while (c != '>' && c != EOF);
            b.valeur[i] = '\0'; 

            if (strstr(b.valeur, "<document>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<document>");
                b.type = DEBUT_DOC;
            } else if (strstr(b.valeur, "</document>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</document>");
                b.type = FIN_DOC;
            } else if (strstr(b.valeur, "<annexe>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<annexe>");
                b.type = DEBUT_ANNEXE;
            } else if (strstr(b.valeur, "</annexe>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</annexe>");
                b.type = FIN_ANNEXE;
            } else if (strstr(b.valeur, "<section>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<section>");
                b.type = DEBUT_SECTION;
            } else if (strstr(b.valeur, "</section>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</section>");
                b.type = FIN_SECTION;
            } else if (strstr(b.valeur, "<titre>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<titre>");
                b.type = DEBUT_TITRE;
            } else if (strstr(b.valeur, "</titre>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</titre>");
                b.type = FIN_TITRE;
            } else if (strstr(b.valeur, "<liste>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<liste>");
                b.type = DEBUT_LISTE;
            } else if (strstr(b.valeur, "</liste>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</liste>");
                b.type = FIN_LISTE;
            } else if (strstr(b.valeur, "<item>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<item>");
                b.type = DEBUT_ITEM;
            } else if (strstr(b.valeur, "</item>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</item>");
                b.type = FIN_ITEM;
            } else if (strstr(b.valeur, "<important>") != NULL) {
                fprintf(fichier_sortie, "(balise_ouvrante, \"%s\")\n", "<important>");
                b.type = DEBUT_IMPORTANT;
            } else if (strstr(b.valeur, "</important>") != NULL) {
                fprintf(fichier_sortie, "(balise_fermante, \"%s\")\n", "</important>");
                b.type = FIN_IMPORTANT;
            } else {
                b.type = 0;
            }
        } else {
            i = 0;
            do {
                b.valeur[i++] = c;
                c = fgetc(fichier_entree);
            } while (c != ' ' && c != '\n' && c != '<' && c != EOF);
            b.valeur[i] = '\0';

            if (strcmp(b.valeur, "retour_a_la_ligne") == 0) {
                b.type = RETOUR_LIGNE;
            } else {
                fprintf(fichier_sortie, "(mot, \"%s\")\n", b.valeur);
                b.type = MOT_SIMPLE;
            }
        }
    }

    return b;
}



int analyse_syntaxique(FILE *fichier_entree, FILE *fichier_sortie) {
    balise b;
    int en_cours = 0;

    b = lire_prochaine_balise(fichier_entree, fichier_sortie);

    while (b.type != 0) {
        if (b.type == DEBUT_DOC) {
            en_cours = FIN_DOC;
        }
        else if (b.type == DEBUT_ANNEXE) {
            en_cours = FIN_ANNEXE;
        }
        else if (b.type == DEBUT_SECTION) {
            en_cours = FIN_SECTION;
        }
        else if (b.type == DEBUT_TITRE) {
            en_cours = FIN_TITRE;
        }
        else if (b.type == DEBUT_LISTE) {
            en_cours = FIN_LISTE;
        }
        else if (b.type == DEBUT_ITEM) {
            en_cours = FIN_ITEM;
        }
        else if (b.type == DEBUT_IMPORTANT) {
            en_cours = FIN_IMPORTANT;
        }
        else if (b.type == FIN_DOC) {
            if (en_cours == FIN_DOC) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else if (b.type == FIN_ANNEXE) {
            if (en_cours == FIN_ANNEXE) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }

         else if (b.type == FIN_SECTION) {
            if (en_cours == FIN_SECTION) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else if (b.type == FIN_TITRE) {
            if (en_cours == FIN_TITRE) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else if (b.type == FIN_LISTE) {
            if (en_cours == FIN_LISTE) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else if (b.type == FIN_ITEM) {
            if (en_cours == FIN_ITEM) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else if (b.type == FIN_IMPORTANT) {
            if (en_cours == FIN_IMPORTANT) {
                en_cours = 0;
            } else {
                return 0; 
            }
        }
        else {
            if (en_cours != 0) {
                return 0; 
            }
        }

        b = lire_prochaine_balise(fichier_entree, fichier_sortie);
    }

    if (en_cours != 0) {
        return 0;
    }

    return 1;
}

int main() {
    FILE *fichier_entree = fopen("input.txt", "r");
    FILE *fichier_sortie = fopen("output.txt", "w");
    if (fichier_entree == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }
    if (fichier_sortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return 1;
    }

    if (analyse_syntaxique(fichier_entree, fichier_sortie)) {
        fprintf(fichier_sortie, "L'analyse syntaxique est réussie\n");
    } else {
        fprintf(fichier_sortie, "Il y a une erreur syntaxique dans le fichier\n");
    }

    fclose(fichier_entree);
    fclose(fichier_sortie);
    return 0;
}