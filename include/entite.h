#pragma once
#include <stdbool.h>
#include "config.h"
#include "terrain.h"

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

typedef struct {
    terrain * terrain;
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
    bool pass;

    // Boucle
    bool launched;
    time_t tempsDebut;

    // Fruit spécial
    time_t tempApparition;
    int posFruitSpecialX;
    int posFruitSpecialY;

    // Paramètre de la partie
    bool fruitInfinie;
    int nombreFruitAjouter;
    int tailleAjouterFruit;
    int tailleAjouterFruitSpecial;
    int scoreFruit;
    int scoreFruitSpecial;
}donneePartie;

entite * creerEntite(int num);
int caseSuivante(entite * ent, terrain * terrain);
void deplacerDirection(entite * ent);
void freeEntite(entite * ent);
