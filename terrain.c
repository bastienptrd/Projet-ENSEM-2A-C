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

void ajouterClasseTerrain(int ** tableau, int largeur, int longueur, int num, int val){
    for (int i = 0; i < largeur; i++){
        for (int j = 0; j < longueur; j++){
            printf("%d, %d (%d, %d)\n", tableau[i][j] / (num * CLASSESERPENT) == 1, tableau[i][j], i, j);
            if (tableau[i][j] / (num * CLASSESERPENT) == 1){
                tableau[i][j] += val;
                if (tableau[i][j] % CLASSESERPENT == 0){
                    tableau[i][j] = VIDE;
                }
            }
        }
    }
}

int nombreCaseObj(int ** tableau, int largeur, int longueur, int obj){
    int somme = 0;
    for (int i = 0; i < largeur; i++){
        for (int j = 0; j < longueur; j++){
            if (tableau[i][j] == obj){
                somme++;
            }
        }
    }
    return somme;
}

void ajouterAleatoireTerrain(int ** tableau, int largeur, int longueur, int nombre, int obj, int* posX, int* posY){
    for (int k = 0; k < nombre; k++){
        int nombreCaseVide = nombreCaseObj(tableau, largeur, longueur, VIDE);

        //Test de cas impossible
        if (nombreCaseVide == 0){
            printf("ERREUR : Pas assez de case pour placer %d", obj);
            return;
        }

        int indexCase = rand() % nombreCaseVide;
        for (int i = 0; i < largeur; i++){
            for (int j = 0; j < longueur; j++){
                if (tableau[i][j] == VIDE && indexCase >= 0){
                    if (indexCase == 0){
                        tableau[i][j] = obj;
                        if (posX != NULL){
                            *posX = i;
                            *posY = j;
                        }
                    }
                    indexCase--;
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