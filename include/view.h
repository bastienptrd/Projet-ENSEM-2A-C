#pragma once
#include "partie.h"

void initWindow();
void afficherTitre(int longueur);
void afficherPseudo(donneePartie * partie);
void afficherDecompte(donneePartie * partie);
void afficherPartie(donneePartie * partie);
void afficherFin(donneePartie * partie, int longueur);
void detruireWindow();
