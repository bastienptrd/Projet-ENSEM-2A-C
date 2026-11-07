#include <stdlib.h>
#include <string.h>

#include "terrain.h"

void initWindow();
void affichageTerrain(terrain * terrain, entite* listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);     //Affichage du terrain et des entites dans le terminal
void printfLongueur(char * message, int longueur, char symb);
void clearAffichage();  //Faire de la place dans le terminal
void afficherTitre(int longueur);
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void afficherPartie(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void afficherFin(int longueur, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite);
void detruireWindow();
