#include "entite.h"

entite * creerEntite(int posX, int posY, int direction, char nom, int num, int longueur){
    //Constructeur d'entite
    entite * ent = malloc(sizeof(entite));
    ent->posX = posX;
    ent->posY = posY;
    ent->direction = direction;
    ent->nom = nom;
    ent->num = num;
    ent->longueur = longueur;
    return ent;
}

int caseSuivante(entite * ent, int ** terrain){
    int caseObserve = 0;
    if (ent->direction == HAUT){
        caseObserve = terrain[ent->posX - 1][ent->posY];
    } else if (ent->direction == BAS){
        caseObserve = terrain[ent->posX + 1][ent->posY];
    } else if (ent->direction == DROITE){
        caseObserve = terrain[ent->posX][ent->posY + 1];
    } else if (ent->direction == GAUCHE){
        caseObserve = terrain[ent->posX][ent->posY - 1];
    }
    return caseObserve;
}

void deplacerDirection(entite * ent){
    if (ent->direction == HAUT){
        ent->posX--;
    } else if (ent->direction == BAS){
        ent->posX++;
    } else if (ent->direction == DROITE){
        ent->posY++;
    } else if (ent->direction == GAUCHE){
        ent->posY--;
    }
}

void freeEntite(entite * ent){
    free(ent);
}