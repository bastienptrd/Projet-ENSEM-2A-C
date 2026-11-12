#pragma once
#include <stdbool.h>
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

typedef struct{
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
    bool pass;
    bool * launched;
} entites;

entite * creerEntite(int num);
int caseSuivante(entite * ent, int ** terrain);
void deplacerDirection(entite * ent);
void freeEntite(entite * ent);
