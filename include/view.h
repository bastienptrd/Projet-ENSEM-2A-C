#pragma once
#include "entite.h"
#include "terrain.h"

void initWindow();
void afficherTitre(int longueur);
void afficherPseudo(entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite, int i, int numEntite);
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite, time_t tempsDebut);
void afficherPartie(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void afficherFin(int longueur, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void detruireWindow();
