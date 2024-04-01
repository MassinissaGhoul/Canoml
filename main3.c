#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
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
    BALISE_RETOUR_LIGNE
} balise_t;

typedef struct {
    FILE* fichier;
    int charac;
    balise_t balise_actuelle; 
    FILE* output;
} element;



void ouverture(element* e, char* nom_fichier);
void fermeture(element* e);
void lire_caractere(element* e);
void passer_espace(element* e);
void update_balise_actuelle(element* e, char* token);
void lire_token(element* e);
void consommer(element* e, char caractere);
void consommer_token(element* e, char* token);
void texte_enrichi(element* e);
void document(element* e);
void contenu(element* e, int balise_contenante);
void section(element* e);
void titre(element* e);
void texte(element* e);
void liste(element* e);
void item(element* e);
void annexes(element* e);
void mot_enrichi(element* e);
void mot_important(element* e);
void mot_simple(element* e);
void fermeture_balise(element* e, int balise_contenante);

void ouverture(element* e, char* nom_fichier){
    e->fichier = fopen(nom_fichier, "r");
    if(e->fichier == NULL){
        exit(-1);
    }
    e->output = fopen("output.txt", "w"); 

    fprintf(e->output, "fdfdfd");
    lire_caractere(e);
}


void fermeture(element* e){
    fclose(e->fichier);
    fclose(e->output); 
}

void lire_caractere(element* e){
    // printf("carac = %c\n", e->charac);
    e->charac = fgetc(e->fichier);
}

void passer_espace(element* e){
    while(e->charac == ' ' || e->charac == '\t' || e->charac == '\n' || e->charac == '\r'){
        lire_caractere(e);
    }
}

balise_t get_balise(const char* token) {
    static const char* balises[] = {
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
        "<br/>"
    };

    for (int i = 0; i < 15; ++i) {
        if (strcmp(token, balises[i]) == 0) {
            return i + 1;
        }
    }

    return BALISE_INCONNUE;
}

void update_balise_actuelle(element* e, char* token) {
    balise_t balise = get_balise(token);

    switch (balise) {
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


void lire_token(element* e) {
    char token[20];
    int i = 0;

    while (e->charac != '>' && e->charac != EOF) {
        token[i] = e->charac;
        lire_caractere(e);
        i++;
    }

    if (e->charac == '>') {
        token[i] = '>'; 
        token[i+1] = '\0'; 
        printf("token AP = %s>\n", token); 
        consommer(e, '>');
        update_balise_actuelle(e, token);
        fprintf(e->output, "%s", token);
    } else {
        printf("Erreur: caractère '>' non trouvé\n");
        exit(-1);
    }
}









void consommer(element* e, char caractere) {
    if (e->charac == caractere) {
        lire_caractere(e);
    }else {
        printf("Erreur: caractere attendu: %c, caractere trouve: %c\n", caractere, e->charac);
        exit(-1);
    }
}

void consommer_token(element* e, char* token){
    int i = 0;
    while (token[i] != '\0') {
        consommer(e, token[i]);
        i++;
    }
    update_balise_actuelle(e, token);
}

void texte_enrichi(element* e){
    document(e);
    while (e->charac != EOF) {
        annexes(e);
    }
}

void document(element* e){
    
    consommer_token(e, "<document>");
    passer_espace(e);
    contenu(e, BALISE_FIN_DOCUMENT);
    passer_espace(e);
}

void contenu(element* e , int balise_contenante){
    while (e->balise_actuelle != balise_contenante) {
        if (e->charac == EOF)
        {
            printf("Erreur: balise de fin manquante\n");
            exit(-1);
        }
        if (e->charac == '<') {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e);
            switch (e->balise_actuelle) {
                case BALISE_FIN_DOCUMENT:
                    fermeture_balise(e, balise_contenante);
                    break;
                case BALISE_FIN_ANNEXE:
                    fermeture_balise(e, balise_contenante);
                    break;
                case BALISE_SECTION:
                    section(e);
                    break;
                case BALISE_FIN_SECTION:
                    fermeture_balise(e, balise_contenante);
                    break;
                case BALISE_TITRE:
                    titre(e);
                    break;
                case BALISE_FIN_TITRE:
                    fermeture_balise(e, balise_contenante);
                    break;
                case BALISE_LISTE:
                    liste(e);
                    passer_espace(e);
                    break;
                case BALISE_FIN_LISTE:
                    fermeture_balise(e, balise_contenante);
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
        else {
            mot_enrichi(e);
        }
    }
    passer_espace(e);
}

void section(element* e){
    passer_espace(e);
    contenu(e, BALISE_FIN_SECTION);
    passer_espace(e);
}

void titre(element* e){
    passer_espace(e);
    texte(e);
    passer_espace(e);
}

void texte(element* e){
    while(e->balise_actuelle != BALISE_FIN_TITRE) {
        if (e->charac == '<') {
            int balise_ancienne = e->balise_actuelle;
            lire_token(e);
            switch (e->balise_actuelle) {
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
        else {
            mot_enrichi(e);
        }
    }
}

void liste(element* e){


    while(e->balise_actuelle != BALISE_FIN_LISTE) {
        passer_espace(e);
        if (e->charac == '<') {

            
            lire_token(e);
            printf("balise actuelle = %d\n", e->balise_actuelle);
            if (e->balise_actuelle == BALISE_FIN_LISTE) {
                fermeture_balise(e, BALISE_FIN_LISTE);
                break;
            }
            else if(e->balise_actuelle == BALISE_ITEM){
                item(e);
                passer_espace(e);
            }
            else {
                printf("Erreur 1: balise liste non fermée\n");
                exit(-1);
            }
        }
    }
}

void item(element* e){
    printf("ITEMO\n");
    while(e->balise_actuelle != BALISE_FIN_ITEM) {
        if (e->charac == '<') {
            lire_token(e);
            if (e->balise_actuelle == BALISE_FIN_ITEM) {
                fermeture_balise(e, BALISE_FIN_ITEM);
                break;
            }
            else if(e->balise_actuelle == BALISE_LISTE){
                liste(e);
            }
            else if(e->balise_actuelle == BALISE_FIN_LISTE){
                break;
            }
            else {
                printf("Erreur 2: balise item non fermée\n");
                exit(-1);
            }
        }
        else {
            mot_enrichi(e);
        }
    }
}

void annexes(element* e){
    consommer_token(e, "<annexe>");
    passer_espace(e);
    contenu(e, BALISE_FIN_ANNEXE);
    passer_espace(e);
}

void mot_enrichi(element* e){
    if (e->balise_actuelle == BALISE_MOT_IMPORTANT) {
        passer_espace(e);
        mot_important(e);
        passer_espace(e);
    } else if(e->balise_actuelle == BALISE_RETOUR_LIGNE) {
        passer_espace(e);
    } else {
        mot_simple(e);
    }
}

void mot_important(element* e){
    while (e->balise_actuelle != BALISE_FIN_MOT_IMPORTANT) {
        if(e->charac == '<') {
            lire_token(e);
            if (e->balise_actuelle == BALISE_FIN_MOT_IMPORTANT) {
                fermeture_balise(e, BALISE_FIN_MOT_IMPORTANT);
                break;
            }
            else {
                printf("Erreur: balise important non fermée\n");
                exit(-1);
            }
        }
        else {
            mot_simple(e);
        }
    }
}

void mot_simple(element* e){
    char mot[100];
    int i = 0;
    while (e->charac != ' ' && e->charac != '\n' && e->charac != '<' && e->charac != EOF) {
        mot[i] = e->charac;
        lire_caractere(e);
        i++;
    }
    mot[i] = '\0';
    fprintf(e->output, "%s", mot); // Modifier cette ligne
    passer_espace(e);
}

void fermeture_balise(element* e, int balise_contenante) {
    if (e->balise_actuelle != balise_contenante) {
        printf("Erreur probleme fermeture: balise attendue: %d, balise trouvee: %d\n", balise_contenante, e->balise_actuelle);
        exit(-1);
    }
}

int main() {
    element e;

    ouverture(&e, "input.txt");

    texte_enrichi(&e);

    fermeture(&e);

    return 0;
}
