#include <stdlib.h>

#include "terrain.h"

void affichage(int** tableau, int largeur, int longeur, entite* listEntite[NOMBREMAXENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void printfDemiLongueur(int tailleMessage, int longueur, char symb);
void clearAffichage();  //Faire de la place dans le terminal
void afficherFin(int longueur, int score, int longeurSerpent, int nombreFruitMange, int nombreFruitSpecialMange, int distanceParcouru);