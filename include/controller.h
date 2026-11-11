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
void gererEvent(entites * entites);
void fermerController();