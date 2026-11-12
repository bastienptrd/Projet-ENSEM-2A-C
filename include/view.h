#pragma once
#include "entite.h"
#include "terrain.h"

void initWindow();
void afficherTitre(int longueur);
void afficherPseudo(entites * entites, int indexLettrePseudo, int numEntite);
void afficherDecompte(terrain * terrain, entites * entites, time_t tempsDebut);
void afficherPartie(terrain * terrain, entites * entites);
void afficherFin(int longueur, entites * entites);
void detruireWindow();
