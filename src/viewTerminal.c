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
static void affichageTerrain(terrain * terrain, entites * entites){
    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < entites->nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (entites->listEntite[k]->posX == i && entites->listEntite[k]->posY == j){
                    printf("%d", entites->listEntite[k]->num);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner, on passe à la suivante
            if (!caseVide){
                continue;
            }

            //Sinon, on dessine le terrain
            if (terrain->tableau[i][j] == VIDE){
                printf("%c", CHAR_VIDE);
            } else if (terrain->tableau[i][j] == MUR){
                printf("%c", CHAR_MURS);
            } else if (terrain->tableau[i][j] == OBSTACLE){
                printf("%c", CHAR_OBSTACLES);
            } else if (terrain->tableau[i][j] == FRUIT){
                printf("%c", CHAR_FRUITS);
            } else if (terrain->tableau[i][j] == FRUIT_SPECIAL){
                printf("%c", CHAR_FRUIT_SPECIAL);
            } else if (terrain->tableau[i][j] > 0) {
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

void afficherPseudo(entites * entites, int indexLettrePseudo, int numEntite) {
    UNUSED(entites);
    UNUSED(indexLettrePseudo);
    UNUSED(numEntite);
}

//Affiche le moment du décompte
void afficherDecompte(terrain * terrain, entites * entites, time_t tempsDebut){
    UNUSED(tempsDebut);

    clearAffichage();
    for (int i = NOMBRE_DECOMPTE; i > 0; i--){
        clearAffichage();
        char message[5];
        sprintf(message, " %d ", i);
        printfLongueur(message, terrain->longueur, '=');
        affichageTerrain(terrain, entites);
        sleep(TEMPS_CHIFFRES_DECOMPTE);
    }
}

//Affiche tous les éléments en milieu de partie
void afficherPartie(terrain * terrain, entites * entites){
    clearAffichage();
    for (int i = 0; i < entites->nombreEntite; i++){
        printf("Score %s : %d", entites->listEntite[i]->pseudo, entites->listEntite[i]->score);
        if (entites->listEntite[i]->evolutionScore > 0){
            printf(" + %d", entites->listEntite[i]->evolutionScore);
        }
        printf("\n");
    }
    affichageTerrain(terrain, entites);
}

//Affiche le message de fin
void afficherFin(int longueur, entites * entites){
    int nombreJoueurEnVie = 0;
    for (int i = 0; i < entites->nombreEntite; i++){
        if (entites->listEntite[i]->enVie){
            nombreJoueurEnVie++;
        }
    }
    
    if (nombreJoueurEnVie == 0 && entites->nombreEntite >= 2){
        printfLongueur(" Partie nulle ", longueur, '=');
    } else if (entites->nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = entites->listEntite[0]->score;
        for (int i = 1; i < entites->nombreEntite; i++){
            if (entites->listEntite[i]->score > scoreMax){
                scoreMax = entites->listEntite[i]->score;
                vainqueur = i;
            }
        }
        char message[30];
        sprintf(message, " %s gagne ", entites->listEntite[vainqueur]->pseudo);
        printfLongueur(message, longueur, '=');
    } else if (nombreJoueurEnVie == 0){
        printfLongueur(" Défaite ", longueur, '=');
    } else {
        printfLongueur(" Victoire ", longueur, '=');
    }

    //Affichage stats
    for (int i = 0; i < entites->nombreEntite; i++){
        printf("Joueur : %s\n", entites->listEntite[i]->pseudo);
        printf("Score : %d\n", entites->listEntite[i]->score);
        printf("Taille serpent : %d\n", entites->listEntite[i]->longueur);
        printf("Nombre de fruit mangé : %d\n", entites->listEntite[i]->nombreFruitMange);
        printf("Nombre de fruit spécial mangé : %d\n", entites->listEntite[i]->nombreFruitSpecialMange);
        printf("Distance parcouru : %d\n\n", entites->listEntite[i]->distanceParcourue);
    }
}

void detruireWindow(){
    printf("\n");
}