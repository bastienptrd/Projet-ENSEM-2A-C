#pragma once

#include "entite.h"

typedef struct {
    terrain * terrain;
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
    bool pass;

    // Boucle
    bool launched;
    time_t tempsDebut;
    int graine;

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

void initialiserPartieEntite(donneePartie * partie);
void initialiserPartieTerrain(donneePartie * partie);
