#include "terrain.h"

int** creerTerrain(int largeur, int longeur){
    //Créer le tableau du terrain
    int ** tableau = malloc(sizeof(int*) * largeur);
    for (int i = 0; i < largeur; i++){
        tableau[i] = malloc(sizeof(int) * longeur);
    }
    return tableau;
}

void initTerrain(int ** tableau, int largeur, int longeur){
    //Place des murs sur toutes les cotés
    for (int i = 0; i < largeur; i++){
        for (int j = 0; j < longeur; j++){
            if (i == 0 || j == 0 || i == largeur - 1 || j == longeur - 1){
                tableau[i][j] = MUR;
            } else {
                tableau[i][j] = VIDE;
            }
        }
    }
}

void updateTerrain(int ** tableau, int largeur, int longeur){
    for (int i = 0; i < largeur; i++){
        for (int j = 0; j < longeur; j++){
            if (tableau[i][j] % CLASSESERPENT > 0){
                tableau[i][j]--;
                if (tableau[i][j] % CLASSESERPENT == 0){
                    tableau[i][j] = VIDE;
                }
            }
        }
    }
}

void freeTerrain(int ** tableau, int largeur){
    //Libération en mémoire du terrain
    for (int i = 0; i < largeur; i++){
        free(tableau[i]);
    }
    free(tableau);
}