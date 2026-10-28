#include <stdlib.h>

#include "terrain.h"

#define CHARMURS '#'
#define CHAROBSTACLES 'X'
#define CHARFRUITS '*'
#define CHARFRUITSPECIAL '@'

#define FREQAFFICHAGE 5 //image par sec

void affichage(int** tableau, int largeur, int longeur, entite* listEntite[NOMBREMAXENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void clearAffichage();  //Faire de la place dans le terminal