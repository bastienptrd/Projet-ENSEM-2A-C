#include "../include/partie.h"

#include <stdio.h>
#include <stdlib.h>

void initialiserPartieEntite(donneePartie * partie) {
    //Initialisation entites
    int indexEntite = 0;
    entite * joueur1 = creerEntite(indexEntite + 1);
    partie->listEntite[indexEntite] = joueur1;
    indexEntite++;

    if (partie->nombreEntite == indexEntite) {
        return;
    }

    entite * joueur2 = creerEntite(indexEntite + 1);
    partie->listEntite[indexEntite] = joueur2;
}

void initialiserPartieTerrain(donneePartie * partie) {
    int largeur = LARGEUR_TERRAIN;
    int longueur = LONGEUR_TERRAIN;

    int nombreMur = NOMBRE_MUR;
    int nombreFruit = NOMBRE_FRUIT;


    // Initialisation terrain
    partie->terrain = creerTerrain(largeur, longueur);
    initTerrain(partie->terrain);
    /*terrain * terrain = malloc(sizeof(terrain));
    terrain->tableau = lireTerrain(CHEMIN_TERRAIN, &largeur, &longueur);
    terrain->largeur = largeur;
    terrain->longueur = longueur;*/

    //Initialisation du random
    partie->graine = time(NULL);
    srand(partie->graine);

    //Ajout des murs
    ajouterAleatoireTerrain(partie->terrain, nombreMur, OBSTACLE, NULL, NULL);

    //Ajout des serpents
    for (int i = 0; i < partie->nombreEntite; i++){
        //Ajouter le serpent (nécessite 2 case vide)
        int nombreEssai = 0;
        do {
            ajouterAleatoireTerrain(partie->terrain, 1, VIDE, &(partie->listEntite[i]->posX), &(partie->listEntite[i]->posY));

            //Retourner erreur en cas de situation bloquante pour la position du seprent
            nombreEssai++;
            if (nombreEssai == NOMBRE_ESSAI_MAX){
                printf("ERREUR : pas assez d'espace pour le serpent");
                return;
            }

        } while (partie->terrain->tableau[partie->listEntite[i]->posX][partie->listEntite[i]->posY] != VIDE || caseSuivante(partie->listEntite[i], partie->terrain) != VIDE);

        //On place le corps du serpents sur le terrain
        partie->terrain->tableau[partie->listEntite[i]->posX][partie->listEntite[i]->posY] = partie->listEntite[i]->longueur + CLASSE_SERPENT * partie->listEntite[i]->num;
    }

    //Ajout des fruits
    ajouterAleatoireTerrain(partie->terrain, nombreFruit, FRUIT, NULL, NULL);

    partie->pass = false;

    //Variables fruit spécial
    partie->tempApparition = -1;

    partie->fruitInfinie = FRUIT_INFINIE;
    partie->nombreFruitAjouter = NOMBRE_FRUIT_AJOUTER;
    partie->tailleAjouterFruit = TAILLE_AJOUTEE_FRUIT;
    partie->tailleAjouterFruitSpecial = TAILLE_AJOUTEE_FRUIT_SPECIAL;
    partie->scoreFruit = SCORE_FRUIT;
    partie->scoreFruitSpecial = SCORE_FRUIT_SPECIAL;
}
