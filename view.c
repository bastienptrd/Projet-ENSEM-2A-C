#include <stdio.h>

#include "view.h"

void affichage(int** tableau, int largeur, int longeur, entite* listEntite[NOMBREMAXENTITE], int nombreEntite){
    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < largeur; i++){
        for (int j = 0; j < longeur; j++){

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (listEntite[k]->posX == i && listEntite[k]->posY == j){
                    printf("%c", listEntite[k]->nom);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner on passe à la suivante
            if (!caseVide){
                continue;
            }

            //Sinon on dessine le terrain
            if (tableau[i][j] == VIDE){
                printf("%c", CHARVIDE);
            } else if (tableau[i][j] == MUR){
                printf("%c", CHARMURS);
            } else if (tableau[i][j] == OBSTACLE){
                printf("%c", CHAROBSTACLES);
            } else if (tableau[i][j] == FRUIT){
                printf("%c", CHARFRUITS);
            } else if (tableau[i][j] == FRUITSPECIAL){
                printf("%c", CHARFRUITSPECIAL);
            } else if (tableau[i][j] > 0) {
                printf("o");
            }
        }
        printf("\n");
    }
}

void printfDemiLongueur(int tailleMessage, int longueur, char symb){
    for (int i = 0; i < (longueur - tailleMessage - 2) / 2; i++){
        printf("%c", symb);
    }
}

void clearAffichage(){
    //Faire de la place dans le terminal
    for (int i = 0; i < NOMBRERETOURCLEAN; i++){
        printf("\n");
    }
}

void afficherFin(int longueur, int score, int longeurSerpent, int nombreFruitMange, int nombreFruitSpecialMange, int distanceParcouru){
    printf("\n");
    printfDemiLongueur(3, longueur, '=');
    printf(" FIN ");
    printfDemiLongueur(3, longueur, '=');
    printf("\n");
    printf("Score : %d\n", score);
    printf("Taille serpent : %d\n", longeurSerpent);
    printf("Nombre de fruit mangé : %d\n", nombreFruitMange);
    printf("Nombre de fruit spécial mangé : %d\n", nombreFruitSpecialMange);
    printf("Distance parcouru : %d\n\n", distanceParcouru);
}