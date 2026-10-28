#include "entite.h"

int** creerTerrain(int largeur, int longeur);    //Créer le tableau du terrain
void initTerrain(int ** tableau, int largeur, int longeur);    //Place des murs sur toutes les cotés
void freeTerrain(int ** tableau, int largeur);    //Libération en mémoire du terrain