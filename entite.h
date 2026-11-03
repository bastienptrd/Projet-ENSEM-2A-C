#ifndef ENTITE_H
#define ENTITE_H

#include "config.h"

typedef struct{
    char pseudo[TAILLEMAXPSEUDO];
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

entite * creerEntite(int num); //Constructeur d'entite
int caseSuivante(entite * ent, int ** terrain);
void deplacerDirection(entite * ent);
void freeEntite(entite * ent);

#endif