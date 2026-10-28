#include "config.h"

typedef struct{
    int posX;
    int posY;
    int direction;
    char nom;
} entite;

entite * creerEntite(int posX, int posY, int direction, char nom); //Constructeur d'entite
void freeEntite(entite * ent);