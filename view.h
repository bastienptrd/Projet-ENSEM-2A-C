#include <stdlib.h>
#include <string.h>

#include "terrain.h"

void affichageTerrain(terrain * terrain, entite* listEntite[NOMBREMAXENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void printfLongueur(char * message, int longueur, char symb);
void clearAffichage();  //Faire de la place dans le terminal
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], int nombreEntite);
void afficherPartie(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], char pseudo[NOMBREMAXENTITE][20], int nombreEntite, int score[NOMBREMAXENTITE], int evolutionScore[NOMBREMAXENTITE]);
void afficherFin(int longueur, int nombreJoueurEnVie, entite * listEntite[NOMBREMAXENTITE], int nombreEntite, char pseudo[NOMBREMAXENTITE][TAILLEMAXPSEUDO], int score[NOMBREMAXENTITE], int nombreFruitMange[NOMBREMAXENTITE], int nombreFruitSpecialMange[NOMBREMAXENTITE], int distanceParcouru[NOMBREMAXENTITE]);
