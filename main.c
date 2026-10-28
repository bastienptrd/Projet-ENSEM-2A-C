#include "view.h"

int main(){

    int largeur = LARGEURTERRAIN; //permettra de faire des tailles variables par la suite
    int longeur = LONGEURTERRAIN; //permettra de faire des tailles variables par la suite

    //initialisation terrain
    int** terrain = creerTerrain(largeur, longeur);
    initTerrain(terrain, largeur, longeur);

    //initialisation entites
    entite* listEntite[NOMBREMAXENTITE];

    int nombreEntite = 0;
    entite * joueur = creerEntite(3, 3, 0, '1');
    listEntite[0] = joueur;
    nombreEntite++;

    //Pour voir les warnings
    sleep(2);

    bool launched = true;

    //Boucle principale
    while (launched){

        //Faire l'affichage
        clearAffichage();
        affichage(terrain, largeur, longeur, listEntite, nombreEntite);

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQAFFICHAGE);
    }

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(terrain, largeur);
    //Les entités
    for (int i = 0; i < nombreEntite; i++){
        free(listEntite[i]);
    }

    return 0;
}