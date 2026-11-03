#include <stdio.h>

#include "view.h"

void affichageTerrain(terrain * terrain, entite* listEntite[NOMBREMAXENTITE], int nombreEntite){
    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (listEntite[k]->posX == i && listEntite[k]->posY == j){
                    printf("%d", listEntite[k]->num);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner on passe à la suivante
            if (!caseVide){
                continue;
            }

            //Sinon on dessine le terrain
            if (terrain->tableau[i][j] == VIDE){
                printf("%c", CHARVIDE);
            } else if (terrain->tableau[i][j] == MUR){
                printf("%c", CHARMURS);
            } else if (terrain->tableau[i][j] == OBSTACLE){
                printf("%c", CHAROBSTACLES);
            } else if (terrain->tableau[i][j] == FRUIT){
                printf("%c", CHARFRUITS);
            } else if (terrain->tableau[i][j] == FRUITSPECIAL){
                printf("%c", CHARFRUITSPECIAL);
            } else if (terrain->tableau[i][j] > 0) {
                printf("%c", CHARSERPENT);
            }
        }
        printf("\n");
    }
}

void printfLongueur(char * message, int longueur, char symb){
    int tailleMessage = strlen(message);
    for (int i = 0; i < (longueur - tailleMessage - 2) / 2; i++){
        printf("%c", symb);
    }
    printf(" %s ", message);
    for (int i = 0; i < (longueur - tailleMessage - 2) / 2; i++){
        printf("%c", symb);
    }
    printf("\n");
}

void clearAffichage(){
    //Faire de la place dans le terminal
    for (int i = 0; i < NOMBRERETOURCLEAN; i++){
        printf("\n");
    }
}

void afficherDecompte(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], int nombreEntite){
    for (int i = NOMBREDECOMPTE; i > 0; i--){
        clearAffichage();
        char message[5];
        sprintf(message, "%d", i);
        printfLongueur(message, terrain->longueur, '=');
        affichageTerrain(terrain, listEntite, nombreEntite);
        sleep(TEMPSCHIFFREDECOMPTE);
    }
}

void afficherPartie(terrain * terrain, entite * listEntite[NOMBREMAXENTITE], int nombreEntite){
    clearAffichage();
    for (int i = 0; i < nombreEntite; i++){
        printf("Score %s : %d", listEntite[i]->pseudo, listEntite[i]->score);
        if (listEntite[i]->evolutionScore > 0){
            printf(" + %d", listEntite[i]->evolutionScore);
        }
        printf("\n");
    }
    affichageTerrain(terrain, listEntite, nombreEntite);
}

void afficherFin(int longueur, entite * listEntite[NOMBREMAXENTITE], int nombreEntite){
    int nombreJoueurEnVie = 0;
    for (int i = 0; i < nombreEntite; i++){
        if (listEntite[i]->enVie){
            nombreJoueurEnVie++;
        }
    }
    
    if (nombreJoueurEnVie == 0 && nombreEntite >= 2){
        printfLongueur("Partie nulle", longueur, '=');
    } else if (nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = listEntite[0]->score;
        for (int i = 1; i < nombreEntite; i++){
            if (listEntite[i]->score > scoreMax){
                scoreMax = listEntite[i]->score;
                vainqueur = i;
            }
        }
        char message[30];
        sprintf(message, "%s gagne", listEntite[vainqueur]->pseudo);
        printfLongueur(message, longueur, '=');
    } else if (nombreJoueurEnVie == 0){
        printfLongueur("Défaite", longueur, '=');
    } else {
        printfLongueur("Victoire", longueur, '=');
    }

    //Affichage du message de fin avec stats
    for (int i = 0; i < nombreEntite; i++){
        printf("Joueur : %s\n", listEntite[i]->pseudo);
        printf("Score : %d\n", listEntite[i]->score);
        printf("Taille serpent : %d\n", listEntite[i]->longueur);
        printf("Nombre de fruit mangé : %d\n", listEntite[i]->nombreFruitMange);
        printf("Nombre de fruit spécial mangé : %d\n", listEntite[i]->nombreFruitSpecialMange);
        printf("Distance parcouru : %d\n\n", listEntite[i]->distanceParcourue);
    }
}