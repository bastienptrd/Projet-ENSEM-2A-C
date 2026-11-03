#include "terrain.h"

//Créer un terrain vide depuis une longueur et une largeur
terrain * creerTerrain(int largeur, int longeur){
    terrain * terrain = malloc(sizeof(terrain));
    //Créer le tableau du terrain
    terrain->tableau = malloc(sizeof(int*) * largeur);
    for (int i = 0; i < largeur; i++){
        terrain->tableau[i] = malloc(sizeof(int) * longeur);
    }
    terrain->largeur = largeur;
    terrain->longueur = longeur;
    return terrain;
}

//Pose des murs sur les bords du terrain
void initTerrain(terrain * terrain){
    //Place des murs sur toutes les cotés
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){
            if (i == 0 || j == 0 || i == terrain->largeur - 1 || j == terrain->longueur - 1){
                terrain->tableau[i][j] = MUR;
            } else {
                terrain->tableau[i][j] = VIDE;
            }
        }
    }
}

//Ajoute val au case dont la classe (cf CLASSE_SERPENT) vaut num  
void ajouterClasseTerrain(terrain * terrain, int num, int val){
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){
            if (terrain->tableau[i][j] / (num * CLASSE_SERPENT) == 1){
                terrain->tableau[i][j] += val;
                if (terrain->tableau[i][j] % CLASSE_SERPENT == 0){
                    terrain->tableau[i][j] = VIDE;
                }
            }
        }
    }
}

//Renvoie le nombre de case à la valeur obj
int nombreCaseObj(terrain * terrain, int obj){
    int somme = 0;
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){
            if (terrain->tableau[i][j] == obj){
                somme++;
            }
        }
    }
    return somme;
}

//Ajout nombre fois de case obj aléatoirement
void ajouterAleatoireTerrain(terrain * terrain, int nombre, int obj, int* posX, int* posY){
    for (int k = 0; k < nombre; k++){
        int nombreCaseVide = nombreCaseObj(terrain, VIDE);

        //Test de cas impossible
        if (nombreCaseVide == 0){
            printf("ERREUR : Pas assez de case pour placer %d", obj);
            return;
        }

        int indexCase = rand() % nombreCaseVide;
        for (int i = 0; i < terrain->largeur; i++){
            for (int j = 0; j < terrain->longueur; j++){
                if (terrain->tableau[i][j] == VIDE && indexCase >= 0){
                    if (indexCase == 0){
                        terrain->tableau[i][j] = obj;
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

//Libération mémoire de terrain
void freeTerrain(terrain * terrain){
    //Libération en mémoire du terrain
    for (int i = 0; i < terrain->largeur; i++){
        free(terrain->tableau[i]);
    }
    free(terrain->tableau);
    free(terrain);
}