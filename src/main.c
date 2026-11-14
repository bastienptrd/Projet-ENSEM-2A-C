#include <stdlib.h>
#include <unistd.h>

#include "../include/gestionFenetre.h"
#include "../include/controller.h"
#include "../include/view.h"

pthread_mutex_t dmutexjoueur1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dmutexjoueur2 = PTHREAD_MUTEX_INITIALIZER;

int main(){
    #pragma region INITIALISATION

    pthread_mutex_t listMutex[NOMBRE_MAX_ENTITE];
    listMutex[0] = dmutexjoueur1;
    listMutex[1] = dmutexjoueur2;

    // Struct qui va contenir toutes les infos liées au déroulement de la partie
    donneePartie * partie = partieVide();

    int fenetre = PASFENETRE;

    struct timespec dernierUpdate, tempsActuelle;
    clock_gettime(CLOCK_MONOTONIC, &dernierUpdate);

    #pragma endregion

    #pragma region BOUCLE PRINCIPALE
    while (partie->launched){

        gererEventFenetre(partie, &fenetre);

        // Calcul de la différence de temps en millisecondes entre la dernière update du plateau et le temps actuelle
        clock_gettime(CLOCK_MONOTONIC, &tempsActuelle);
        double dt = ((tempsActuelle.tv_sec - dernierUpdate.tv_sec) * 1000.0) + ((tempsActuelle.tv_nsec - dernierUpdate.tv_nsec) / 1000000.0); //Aider par IA temps en ms

        if (fenetre == JEU && dt > 1000 / (float)FREQ_DEPLACEMENT) {
            clock_gettime(CLOCK_MONOTONIC, &dernierUpdate);

            // On comptabilise les scores du tour précédent
            incrementerScore(partie);

            deplacementSerpents(partie, listMutex);

            updateFruitSpecial(partie);
        }

        afficherFenetre(partie, &fenetre);

        usleep(1000000 / FREQ_AFFICHAGE);
    }
    #pragma endregion

    fermerController();

    detruireWindow();

    libererPartie(partie);

    return 0;
}