#include <stdlib.h>
#include <string.h>

#include "terrain.h"

void affichage(int** tableau, int largeur, int longeur, entite* listEntite[NOMBREMAXENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void printfLongueur(char * message, int longueur, char symb);
void clearAffichage();  //Faire de la place dans le terminal
void afficherFin(int score, int longeurSerpent, int nombreFruitMange, int nombreFruitSpecialMange, int distanceParcouru);