#include <stdio.h>
#include <stdlib.h>


FILE* mon_fichier;
char mon_caractere;


void lire_caractere(){
    mon_caractere = fgetc(mon_fichier);
    printf("%c", mon_caractere);
}

void passer_espace(){
    while(mon_caractere == ' ' || mon_caractere == '\t' || mon_caractere == '\n' || mon_caractere == '\r'){
        lire_caractere();
    }
}

void amorcer(char* nom_fichier){
    mon_fichier = fopen (nom_fichier, "r");
    if(mon_fichier == NULL){
        exit(-1);
    }
    lire_caractere();
    passer_espace();
}



