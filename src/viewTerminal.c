#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../include/configTerminal.h"
#include "../include/view.h"
#include "../include/utilitaire.h"

void initWindow(){
    printf("\n");
}

//Affiche le plateau avec tous ces éléments
static void affichageTerrain(donneePartie * partie){
    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < partie->terrain->largeur; i++){
        for (int j = 0; j < partie->terrain->longueur; j++){

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < partie->nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (partie->listEntite[k]->posX == i && partie->listEntite[k]->posY == j){
                    printf("%d", partie->listEntite[k]->num);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner, on passe à la suivante
            if (!caseVide){
                continue;
            }

            //Sinon, on dessine le terrain
            if (partie->terrain->tableau[i][j] == VIDE){
                printf("%c", CHAR_VIDE);
            } else if (partie->terrain->tableau[i][j] == MUR){
                printf("%c", CHAR_MURS);
            } else if (partie->terrain->tableau[i][j] == OBSTACLE){
                printf("%c", CHAR_OBSTACLES);
            } else if (partie->terrain->tableau[i][j] == FRUIT){
                printf("%c", CHAR_FRUITS);
            } else if (partie->terrain->tableau[i][j] == FRUIT_SPECIAL){
                printf("%c", CHAR_FRUIT_SPECIAL);
            } else if (partie->terrain->tableau[i][j] > 0) {
                printf("%c", CHAR_SERPENT);
            }
        }
        printf("\n");
    }
}

//Fait un printf centré par rapport à longueur
static void printfLongueur(char * message, int longueur, char symb){
    int tailleMessage = strlen(message);
    for (int i = 0; i < (longueur - tailleMessage) / 2; i++){
        printf("%c", symb);
    }
    printf("%s", message);
    for (int i = 0; i < (longueur - tailleMessage) / 2; i++){
        printf("%c", symb);
    }
    if ((longueur - tailleMessage) % 2){
        printf("%c", symb);
    }
    printf("\n");
}

//Fait NOMBRE_RETOUR_CLEAN retour à la ligne pour faire de la place dans le terminal
static void clearAffichage(){
    for (int i = 0; i < NOMBRE_RETOUR_CLEAN; i++){
        printf("\n");
    }
}

//Affiche SNAKE dans le terminal
void afficherTitre(int longueur){
    longueur = max(longueur, 34);
    printfLongueur("", longueur, '=');
    //txt fait avec l'IA
    printfLongueur("  ____  _   _    _    _  _______  ", longueur, '=');
    printfLongueur(" / ___|| \\ | |  / \\  | |/ / ____| ", longueur, '=');
    printfLongueur(" \\___ \\|  \\| | / _ \\ | ' /|  _|   ", longueur, '=');
    printfLongueur("  ___) | |\\  |/ ___ \\| . \\| |___  ", longueur, '=');
    printfLongueur(" |____/|_| \\_/_/   \\_\\_|\\_\\_____| ", longueur, '=');

    printf("\n");
    printfLongueur("", longueur, '=');
}

void afficherPseudo(donneePartie * partie, int indexLettrePseudo, int numEntite) {
    UNUSED(partie);
    UNUSED(indexLettrePseudo);
    UNUSED(numEntite);
}

//Affiche le moment du décompte
void afficherDecompte(donneePartie * partie){
    clearAffichage();
    for (int i = NOMBRE_DECOMPTE; i > 0; i--){
        clearAffichage();
        char message[5];
        sprintf(message, " %d ", i);
        printfLongueur(message, partie->terrain->longueur, '=');
        affichageTerrain(partie);
        sleep(TEMPS_CHIFFRES_DECOMPTE);
    }
}

//Affiche tous les éléments en milieu de partie
void afficherPartie(donneePartie * partie){
    clearAffichage();
    for (int i = 0; i < partie->nombreEntite; i++){
        printf("Score %s : %d", partie->listEntite[i]->pseudo, partie->listEntite[i]->score);
        if (partie->listEntite[i]->evolutionScore > 0){
            printf(" + %d", partie->listEntite[i]->evolutionScore);
        }
        printf("\n");
    }
    affichageTerrain(partie);
}

//Affiche le message de fin
void afficherFin(donneePartie * partie, int longueur){
    int nombreJoueurEnVie = 0;
    for (int i = 0; i < partie->nombreEntite; i++){
        if (partie->listEntite[i]->enVie){
            nombreJoueurEnVie++;
        }
    }
    
    if (nombreJoueurEnVie == 0 && partie->nombreEntite >= 2){
        printfLongueur(" Partie nulle ", longueur, '=');
    } else if (partie->nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = partie->listEntite[0]->score;
        for (int i = 1; i < partie->nombreEntite; i++){
            if (partie->listEntite[i]->score > scoreMax){
                scoreMax = partie->listEntite[i]->score;
                vainqueur = i;
            }
        }
        char message[30];
        sprintf(message, " %s gagne ", partie->listEntite[vainqueur]->pseudo);
        printfLongueur(message, longueur, '=');
    } else if (nombreJoueurEnVie == 0){
        printfLongueur(" Défaite ", longueur, '=');
    } else {
        printfLongueur(" Victoire ", longueur, '=');
    }

    //Affichage stats
    for (int i = 0; i < partie->nombreEntite; i++){
        printf("Joueur : %s\n", partie->listEntite[i]->pseudo);
        printf("Score : %d\n", partie->listEntite[i]->score);
        printf("Taille serpent : %d\n", partie->listEntite[i]->longueur);
        printf("Nombre de fruit mangé : %d\n", partie->listEntite[i]->nombreFruitMange);
        printf("Nombre de fruit spécial mangé : %d\n", partie->listEntite[i]->nombreFruitSpecialMange);
        printf("Distance parcouru : %d\n\n", partie->listEntite[i]->distanceParcourue);
    }
}

void detruireWindow(){
    printf("\n");
}