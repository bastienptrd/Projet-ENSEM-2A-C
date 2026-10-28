#include "view.h"
#include "controller.h"

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    //Initialisation du random
    srand(time(0));

    int largeur = LARGEURTERRAIN; //permettra de faire des tailles variables par la suite
    int longeur = LONGEURTERRAIN; //permettra de faire des tailles variables par la suite

    //initialisation terrain
    int** terrain = creerTerrain(largeur, longeur);
    initTerrain(terrain, largeur, longeur);

    //initialisation entites
    entite* listEntite[NOMBREMAXENTITE];

    int nombreEntite = 0;
    entite * joueur = creerEntite(3, 3, 0, '1', nombreEntite + 1, TAILLEINITIALESERPENT);
    listEntite[0] = joueur;
    nombreEntite++;

    clearAffichage();
    affichage(terrain, largeur, longeur, listEntite, nombreEntite);

    //Pour voir les warnings debbug à supprimer à la fin
    sleep(2);

    //Decompte vers le début
    for (int i = 3; i > 0; i--){
        clearAffichage();
        printf("%d\n", i);
        affichage(terrain, largeur, longeur, listEntite, nombreEntite);
        sleep(1);
    }
    
    //Recuperation des entrées clavier
    pthread_t clavier;
    pthread_create(&clavier,NULL,lire_clavier,joueur);

    bool launched = true;

    //Boucle principale
    while (launched){

        //On "déplace" le corps du serpent
        updateTerrain(terrain, largeur, longeur);

        //On verrouille le mutex pour eviter des changement de direction entre le teste et le déplacement
        pthread_mutex_lock(&dmutex);
        if (peutDeplacerDirection(joueur, terrain)){
            terrain[joueur->posX][joueur->posY] = joueur->longueur - 1 + CLASSESERPENT * joueur->num;
            deplacerDirection(joueur);
        } else {
            launched = false;
        }
        pthread_mutex_unlock(&dmutex);

        //Faire l'affichage
        clearAffichage();
        affichage(terrain, largeur, longeur, listEntite, nombreEntite);

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQAFFICHAGE);
    }

    printf("mort\n");
    
    //On arrete le thread clavier
    pthread_cancel(clavier);
    pthread_join(clavier, NULL);

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(terrain, largeur);
    //Les entités
    for (int i = 0; i < nombreEntite; i++){
        free(listEntite[i]);
    }

    return 0;
}