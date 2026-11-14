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

    // Gestion écriture pseudo
    int indexLettrePseudo;
    int indexEntitePseudo;

    // Fruit spécial
    time_t tempApparitionFruitSpecial;
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

donneePartie * partieVide();
void initialiserPartieEntite(donneePartie * partie);
void initialiserPartieTerrain(donneePartie * partie);
void incrementerScore(donneePartie * partie);
void deplacementSerpents(donneePartie * partie, pthread_mutex_t listMutex[NOMBRE_MAX_ENTITE]);
void updateFruitSpecial(donneePartie * partie);
void testFin(donneePartie * partie, int * fenetre);
void enregistrerPartie(donneePartie * partie);
void libererPartie(donneePartie * partie);
