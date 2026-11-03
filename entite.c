#include "entite.h"

//Constructeur d'entite
entite * creerEntite(int num){
    entite * ent = malloc(sizeof(entite));

    ent->num = num;
    ent->enVie = true;

    ent->posX = 0;
    ent->posY = 0;
    ent->direction = DIRECTION_DEFAULT_SERPENT;
    
    ent->score = 0;
    ent->evolutionScore = 0;

    ent->longueur = TAILLE_INITIALE_SERPENT;
    ent->distanceParcourue = 0;
    ent->nombreFruitMange = 0;
    ent->nombreFruitSpecialMange = 0;
    return ent;
}

//Donne la case située devant l'entité
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

//Déplace l'entité dans la direction visée
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

//Libération mémoire d'entité
void freeEntite(entite * ent){
    free(ent);
}