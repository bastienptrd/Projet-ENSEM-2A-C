#ifndef ENTITE_H
#define ENTITE_H

#include "config.h"

typedef struct{
    char pseudo[TAILLE_MAX_PSEUDO];
    int num;
    bool enVie;

    int posX;
    int posY;
    int direction;
    
    int score;
    int evolutionScore;

    int longueur;
    int distanceParcourue;
    int nombreFruitMange;
    int nombreFruitSpecialMange;
} entite;

entite * creerEntite(int num);
int caseSuivante(entite * ent, int ** terrain);
void deplacerDirection(entite * ent);
void freeEntite(entite * ent);

#endif