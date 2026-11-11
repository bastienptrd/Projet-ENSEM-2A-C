#pragma once

#include "entite.h"

typedef struct{
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
    bool pass;
    bool * launched;
} entites;

void initialiserController(entites * entites);
void lirePseudo(char pseudo[TAILLE_MAX_PSEUDO]);
void gererEventMenu(entites * entites, int * fenetre);
void gererEventPseudo(entites * entites, int * fenetre, int * i, int * numEntite);
void gererEventDecompte(entites * entites, int * fenetre, time_t tempsDebut);
void gererEventJeu(entites * entites, int * fenetre);
void gererEventFin(entites * entites, int * fenetre);
void fermerController();