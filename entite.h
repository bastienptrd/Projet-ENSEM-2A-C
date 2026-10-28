#ifndef ENTITE_H
#define ENTITE_H

#include "config.h"

typedef struct{
    int posX;
    int posY;
    int direction;
    char nom;
    int num;
    int longueur;
} entite;

entite * creerEntite(int posX, int posY, int direction, char nom, int num, int longueur); //Constructeur d'entite
int caseSuivante(entite * ent, int ** terrain);
void deplacerDirection(entite * ent);
void freeEntite(entite * ent);

#endif