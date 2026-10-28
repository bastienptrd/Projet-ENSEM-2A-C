#include "entite.h"

entite * creerEntite(int posX, int posY, int direction, char nom){
    //Constructeur d'entite
    entite * ent = malloc(sizeof(entite));
    ent->posX = posX;
    ent->posY = posY;
    ent->direction = direction;
    ent->nom = nom;
    return ent;
}

void freeEntite(entite * ent){
    free(ent);
}