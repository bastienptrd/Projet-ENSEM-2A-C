#include <stdlib.h>
#include <string.h>

#include "terrain.h"

void affichageTerrain(terrain * terrain, entite* listEntite[NOMBREMAXENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void printfLongueur(char * message, int longueur, char symb);
void clearAffichage();  //Faire de la place dans le terminal
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], int nombreEntite);
void afficherPartie(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], int nombreEntite);
void afficherFin(int longueur, entite * listEntite[NOMBREMAXENTITE], int nombreEntite);
