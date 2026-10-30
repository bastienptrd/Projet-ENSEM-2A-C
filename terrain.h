#include "entite.h"

int** creerTerrain(int largeur, int longeur);    //Créer le tableau du terrain
void initTerrain(int ** tableau, int largeur, int longeur);    //Place des murs sur toutes les cotés
void ajouterClasseTerrain(int ** tableau, int largeur, int longeur, int num, int val);
void ajouterAleatoireTerrain(int ** tableau, int largeur, int longueur, int nombre, int obj, int* posX, int* posY);
void freeTerrain(int ** tableau, int largeur);    //Libération en mémoire du terrain