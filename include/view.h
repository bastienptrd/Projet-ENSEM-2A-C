#include <stdlib.h>
#include <string.h>

#include "terrain.h"

void initWindow();
void afficherTitre(int longueur);
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void afficherPartie(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void afficherFin(int longueur, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void detruireWindow();
