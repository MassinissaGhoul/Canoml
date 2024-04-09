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
    BALISE_TEXTE // Ajouter cette ligne
} balise_t;

typedef struct Node
{
    balise_t balise;
    char *contenu;
    int nb_enfants;
    struct Node **enfants;
    struct Node *parent;
    struct Node *frere;
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

Node *creer_noeud(balise_t balise, char *contenu);
void ajouter_enfant(Node *parent, Node *enfant);
void ecrire_arbre(FILE *output, Node *noeud);

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
void ajouter_frere(Node *noeud, Node *frere);
// void afficher_arbre_avec_indentation(Node* noeud, char* prefix, FILE* fichier, int estDernier);
void afficher_arbre(Node *noeud, int profondeur, FILE *fichier);
void printNodeData(Node *noeud, int level, Node *parent);
void printBox(Node *node, int level, FILE *fichier);
char *getNodeContent(Node *noeud);
void afficher_arbre_box(Node *noeud, int profondeur, FILE *fichier);



Node *creer_noeud(balise_t balise, char *contenu)
{
    Node *noeud = malloc(sizeof(Node));
    noeud->balise = balise;
    noeud->contenu = contenu;
    noeud->nb_enfants = 0;
    noeud->enfants = NULL;
    noeud->parent = NULL;
    return noeud;
}

void ajouter_frere(Node *noeud, Node *frere)
{
    frere->parent = noeud->parent;
    frere->frere = noeud->frere;
    noeud->frere = frere;
}

void ajouter_enfant(Node *parent, Node *enfant)
{
    parent->nb_enfants++;
    parent->enfants = realloc(parent->enfants, sizeof(Node *) * parent->nb_enfants);
    parent->enfants[parent->nb_enfants - 1] = enfant;
    enfant->parent = parent;
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

void ouverture(element *e, char *nom_fichier)
{
    e->fichier = fopen(nom_fichier, "r");
    if (e->fichier == NULL)
    {
        exit(-1);
    }
    e->output = fopen("output.txt", "w");
    fprintf(e->output, "fdfdfd");
    lire_caractere(e);
}

void fermeture(element *e)
{
    ecrire_arbre(e->output, e->noeud_courant);
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
        printf("token AP = %s>\n", token);
        consommer(e, '>');
        update_balise_actuelle(e, token);
        fprintf(e->output, "%s", token);
        char *contenu = strdup(token);

        printf(" ICICICIIC%s\n", token);
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
        exit(-1);
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
        annexes(e);
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
    while (e->balise_actuelle != balise_contenante)
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
    fprintf(e->output, "%s ", contenu);
    passer_espace(e);
}

void fermeture_balise(element *e, int balise_contenante) {
    printf("Fermeture balise attendue: %d, balise courante: %d\n", balise_contenante, e->noeud_courant->balise);
    while (e->noeud_courant->balise != balise_contenante) {
        if (e->noeud_courant->parent == NULL) {
            printf("Erreur: balise de fin manquante pour %d\n", balise_contenante);
            exit(-1);
        }
        e->noeud_courant = e->noeud_courant->parent;
    }
    if (e->noeud_courant->parent != NULL) {
        e->noeud_courant = e->noeud_courant->parent; // Remonter au parent
    } else {
        printf("Avertissement: la balise de fin pour %d est à la racine.\n", balise_contenante);
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
// void afficher_arbre(Node* noeud, FILE* fichier, int profondeur, int dernier_enfant) {
//     if (noeud == NULL) {
//         return;
//     }

//     char symbole = dernier_enfant ? '└' : '├';
//     char remplissage = dernier_enfant ? ' ' : '|';

//     for (int i = 0; i < profondeur; i++) {
//         fprintf(fichier, "%c   ", remplissage);
//     }

//     switch (noeud->balise) {
//         case BALISE_DOCUMENT:
//             fprintf(fichier, "%c── document\n", symbole);
//             break;
//         case BALISE_FIN_DOCUMENT:
//             break;
//         case BALISE_ANNEXE:
//             fprintf(fichier, "%c── annexe\n", symbole);
//             break;
//         case BALISE_FIN_ANNEXE:
//             break;
//         case BALISE_TITRE:
//             fprintf(fichier, "%c── titre\n", symbole);
//             break;
//         case BALISE_FIN_TITRE:
//             break;
//         case BALISE_LISTE:
//             fprintf(fichier, "%c── liste\n", symbole);
//             break;
//         case BALISE_FIN_LISTE:
//             break;
//         case BALISE_MOT_IMPORTANT:
//             fprintf(fichier, "%c── mot_important: %s\n", symbole, noeud->contenu);
//             break;
//         case BALISE_FIN_MOT_IMPORTANT:
//             break;
//         case BALISE_SECTION:
//             fprintf(fichier, "%c── section\n", symbole);
//             break;
//         case BALISE_FIN_SECTION:
//             break;
//         case BALISE_ITEM:
//             fprintf(fichier, "%c── item\n", symbole);
//             break;
//         case BALISE_FIN_ITEM:
//             break;
//         case BALISE_RETOUR_LIGNE:
//             break;
//         case BALISE_TEXTE:
//             fprintf(fichier, "%c── %s\n", symbole, noeud->contenu);
//             break;
//         default:
//             fprintf(fichier, "%c── balise inconnue\n", symbole);
//             break;
//     }

//     for (int i = 0; i < noeud->nb_enfants; i++) {
//         afficher_arbre(noeud->enfants[i], fichier, profondeur + 1, i == noeud->nb_enfants - 1);
//     }
// }

char *getNodeContent(Node *noeud)
{
    if (!noeud)
        return "Nœud Vide";

    static char content[256];
    memset(content, 0, sizeof(content));

    switch (noeud->balise)
    {
    case BALISE_DOCUMENT:
        strcpy(content, "Document");
        break;
    case BALISE_ANNEXE:
        strcpy(content, "Annexe");
        break;
    case BALISE_TITRE:
        strcpy(content, "Titre");
        break;
    case BALISE_LISTE:
        strcpy(content, "Liste");
        break;
    case BALISE_ITEM:
        strcpy(content, "Item");
        break;
    case BALISE_SECTION:
        strcpy(content, "Section");
        break;
    case BALISE_MOT_IMPORTANT:
        sprintf(content, "Mot Important: %s", noeud->contenu);
        break;
    case BALISE_TEXTE:
        sprintf(content, "%s", noeud->contenu);
        break;
    default:
        strcpy(content, "Type Inconnu");
        break;
    }

    return content;
}

void printBox(Node *node, int level, FILE *fichier)
{
    char indent[1024] = {0};
    for (int i = 0; i < level; ++i)
    {
        strcat(indent, "  ");
    }

    fprintf(fichier, "%s+----------------------------------------------+\n", indent);

    char *content = getNodeContent(node);
    fprintf(fichier, "%s|%s|\n", indent, content);

    for (int i = 0; i < node->nb_enfants; ++i)
    {
        printBox(node->enfants[i], level + 1, fichier);
    }

    fprintf(fichier, "%s+----------------------------------------------+\n", indent);
}

void printNodeData(Node *noeud, int level, Node *parent)
{
    for (int i = 0; i < level; ++i)
    {
        printf("  ");
    }

    if (noeud->contenu != NULL)
    {
        printf("Niveau %d, Balise: %d, Contenu: %s, Parent: ", level, noeud->balise, noeud->contenu);
        if (parent != NULL)
        {
            printf("Niveau %d, Balise: %d, Contenu: %s\n", level - 1, parent->balise, parent->contenu);
        }
        else
        {
            printf("Racine\n");
        }
    }
    else
    {
        printf("Niveau %d, Balise: %d, Contenu: Vide, Parent: ", level, noeud->balise);
        if (parent != NULL)
        {
            printf("Niveau %d, Balise: %d, Contenu: %s\n", level - 1, parent->balise, parent->contenu);
        }
        else
        {
            printf("Racine\n");
        }
    }

    for (int i = 0; i < noeud->nb_enfants; ++i)
    {
        printNodeData(noeud->enfants[i], level + 1, noeud);
    }
}


void afficher_arbre_box(Node *noeud, int profondeur, FILE *fichier)
{
    if (noeud == NULL)
    {
        return;
    }

    int i;
    for (i = 0; i < profondeur; i++)
    {
        fprintf(fichier, "|  ");
    }

    switch (noeud->balise)
    {
    case BALISE_DOCUMENT:
        fprintf(fichier, "+------------------------------------------------+\n");
        fprintf(fichier, "| DOCUMENT                                       |\n", noeud->contenu);
        fprintf(fichier, "+------------------------------------------------+\n");
        break;
    case BALISE_TITRE:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| TITRE                                        |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_SECTION:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| SECTION                                      |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_LISTE:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| LISTE                                        |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_ITEM:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| ITEM                                         |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_ANNEXE:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| ANNEXE                                       |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_MOT_IMPORTANT:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| Mot important : %s                           |\n", noeud->contenu);
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    case BALISE_RETOUR_LIGNE:
        break;
    case BALISE_FIN_DOCUMENT:
        break;
    case BALISE_FIN_TITRE:
        break;
    case BALISE_FIN_SECTION:
        break;
    case BALISE_FIN_LISTE:
        break;
    case BALISE_FIN_ITEM:
        break;
    case BALISE_FIN_ANNEXE:
        break;
    case BALISE_TEXTE:
        fprintf(fichier, "| %s |\n", noeud->contenu);
        break;
    default:
        fprintf(fichier, "+----------------------------------------------+\n");
        fprintf(fichier, "| Balise inconnue                              |\n");
        fprintf(fichier, "+----------------------------------------------+\n");
        break;
    }

    for (i = 0; i < noeud->nb_enfants; i++)
    {
        afficher_arbre_box(noeud->enfants[i], profondeur + 1, fichier);
    }

    if (noeud->balise == BALISE_TEXTE)
    {
        for (i = 0; i < profondeur; i++)
        {
            fprintf(fichier, "|  ");
        }
        fprintf(fichier, "+\n");
    }
}



























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
    afficher_arbre_box(e.noeud_racine, 0, fichier2);

    fclose(fichier2);

    fclose(fichier);
    return 0;
}



