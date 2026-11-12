#pragma once
#include "entite.h"

void initWindow();
void afficherTitre(int longueur);
void afficherPseudo(donneePartie * partie, int indexLettrePseudo, int numEntite);
void afficherDecompte(donneePartie * partie);
void afficherPartie(donneePartie * partie);
void afficherFin(donneePartie * partie, int longueur);
void detruireWindow();
