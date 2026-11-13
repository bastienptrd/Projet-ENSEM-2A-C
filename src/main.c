#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/view.h"
#include "../include/controller.h"
#include "../include/gestionFichier.h"

pthread_mutex_t dmutexjoueur1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dmutexjoueur2 = PTHREAD_MUTEX_INITIALIZER;

int main(){
    #pragma region INITIALISATION

    pthread_mutex_t listMutex[NOMBRE_MAX_ENTITE];
    listMutex[0] = dmutexjoueur1;
    listMutex[1] = dmutexjoueur2;

    // Struct qui va contenir toutes les infos liées au déroulement de la partie
    donneePartie partie;

    //Condition de la boucle principale
    partie.launched = true;

    partie.nombreEntite = NOMBRE_MAX_ENTITE;
    for (int i = 0; i < NOMBRE_MAX_ENTITE; i++) {
        partie.listEntite[i] = NULL;
    }
    partie.terrain = NULL;

    int fenetre = START;

    int rangPseudo = 0;
    int numEntitePseudo = 0;

    struct timespec dernierUpdate, tempsActuelle;
    clock_gettime(CLOCK_MONOTONIC, &dernierUpdate);

    //Permet d'arreter la boucle au besoin
    int nombreJoueurMort = 0;

    #pragma endregion

    initWindow();

    #pragma region BOUCLE PRINCIPALE
    while (partie.launched){

        #pragma region GESTION EVENT
        if (fenetre == START) {
            // Chargement
            fenetre = MENU;
        }else if (fenetre == MENU) {
            gererEventMenu(&partie, &fenetre);
            if (fenetre == PSEUDO) {
                initialiserPartieEntite(&partie);
            }
        } else if (fenetre == PSEUDO) {
            gererEventPseudo(&partie, &fenetre, &rangPseudo, &numEntitePseudo);
            if (fenetre == DECOMPTE) {

                initialiserPartieTerrain(&partie);

                time(&partie.tempsDebut);

                initialiserController(&partie);

                //Selectionne une direction
                for (int i = 0; i < partie.nombreEntite; i++){
                    partie.listEntite[i]->direction = DIRECTION_DEFAULT_SERPENT;
                }

                //Autorise les joueurs à changer librement de direction avant le début
                partie.pass = true;
            }
        } else if (fenetre == DECOMPTE) {
            gererEventDecompte(&partie, &fenetre);
            if (fenetre == JEU) {
                //Retire authorisation du changement libre
                partie.pass = false;
            }
        } else if (fenetre == JEU) {
            gererEventJeu(&partie, &fenetre);
        } else if (fenetre == FIN) {
            gererEventFin(&partie, &fenetre);
        }
        #pragma endregion

        #pragma region UPDATE PLATEAU

        // Calcul de la différence de temps en millisecondes entre la dernière update du plateau et le temps actuelle
        clock_gettime(CLOCK_MONOTONIC, &tempsActuelle);
        double dt = ((tempsActuelle.tv_sec - dernierUpdate.tv_sec) * 1000.0) + ((tempsActuelle.tv_nsec - dernierUpdate.tv_nsec) / 1000000.0); //Aider par IA temps en ms

        if (fenetre == JEU && dt > 1000 / (float)FREQ_DEPLACEMENT) {
            clock_gettime(CLOCK_MONOTONIC, &dernierUpdate);

            #pragma region SCORE
            //Calculs des scores du passage precédent (laisse le temps à l'affichage)
            for (int i = 0; i < partie.nombreEntite; i++){
                partie.listEntite[i]->score += partie.listEntite[i]->evolutionScore;
                partie.listEntite[i]->evolutionScore = 0;
            }
            #pragma endregion

            #pragma region DEPLACEMENT SERPENT
            //Déplacement des serpents
            for (int i = 0; i < partie.nombreEntite; i++){
                //Si le joueur est mort, on passe au suivant
                if (partie.listEntite[i]->enVie == 0){
                    continue;
                }

                //On verrouille le mutex pour eviter des changements de direction entre le teste et le déplacement
                pthread_mutex_lock(listMutex + i);

                //On regarde la prochaine case atteinte par le serpent
                int caseDirection = caseSuivante(partie.listEntite[i], partie.terrain);

                //Si c'est un obstacle
                if (caseDirection > CLASSE_SERPENT || caseDirection == MUR || caseDirection == OBSTACLE){
                    partie.listEntite[i]->enVie = false;
                    nombreJoueurMort++;
                    pthread_mutex_unlock(listMutex + i);
                    continue;
                }

                //Si la case n'est pas "solide"
                //On "déplace" le corps du serpent
                ajouterClasseTerrain(partie.terrain, partie.listEntite[i]->num, -1);

                //Si on rencontre un fruit, on agrandit le serpent et on fait évoluer le score
                if (caseDirection == FRUIT){
                    partie.listEntite[i]->evolutionScore = partie.scoreFruit;
                    partie.listEntite[i]->nombreFruitMange++;
                    ajouterClasseTerrain(partie.terrain, partie.listEntite[i]->num, partie.tailleAjouterFruit);
                    partie.listEntite[i]->longueur++;

                    if (partie.fruitInfinie){
                        //On rajoute un fruit sur le terrain
                        ajouterAleatoireTerrain(partie.terrain, partie.nombreFruitAjouter, FRUIT, NULL, NULL);
                    }
                } else if (caseDirection == FRUIT_SPECIAL){
                    partie.listEntite[i]->evolutionScore = partie.scoreFruitSpecial;
                    partie.listEntite[i]->nombreFruitSpecialMange++;
                    ajouterClasseTerrain(partie.terrain, partie.listEntite[i]->num, partie.tailleAjouterFruitSpecial);
                    partie.listEntite[i]->longueur++;
                    partie.tempApparition = -1;
                }

                //On déplace la tête
                deplacerDirection(partie.listEntite[i]);

                pthread_mutex_unlock(listMutex + i);

                //On pose une partie du corp du serpent
                partie.terrain->tableau[partie.listEntite[i]->posX][partie.listEntite[i]->posY] = partie.listEntite[i]->longueur + CLASSE_SERPENT * partie.listEntite[i]->num;

                partie.listEntite[i]->distanceParcourue++;
            }
            #pragma endregion

            #pragma region FRUIT SPECIAL
            //On fait les tests sur le fruit spécial
            if (partie.tempApparition == -1 && rand() % FREQ_APPARITION_FRUIT_SPECIAL * FREQ_AFFICHAGE == 0){
                partie.tempApparition = time(NULL);
                ajouterAleatoireTerrain(partie.terrain, 1, FRUIT_SPECIAL, &partie.posFruitSpecialX, &partie.posFruitSpecialY);
            } else if (partie.tempApparition != -1 && difftime(time(NULL), partie.tempApparition) > TEMP_APPARITION_FRUIT_SPECIAL){
                partie.terrain->tableau[partie.posFruitSpecialX][partie.posFruitSpecialY] = VIDE;
                partie.tempApparition = -1;
            }
            #pragma endregion
        }
        #pragma endregion

        #pragma region AFFICHAGE

        if (fenetre == MENU) {
            afficherTitre(LONGEUR_TERRAIN);
        } else if (fenetre == PSEUDO) {
            afficherPseudo(&partie, rangPseudo, numEntitePseudo);
        } else if (fenetre == DECOMPTE) {
            afficherDecompte(&partie);
        } else if (fenetre == JEU) {
            afficherPartie(&partie);
        } else if (fenetre == FIN) {
            afficherFin(&partie, partie.terrain->longueur);
        }

        #pragma endregion

        if (fenetre == JEU) {

            #pragma region TESTS FIN
            //Test fin de partie
            //Si on a mangé tous les fruits
            if (nombreCaseObj(partie.terrain, FRUIT) == 0){
                fenetre = FIN;
            }
            //Si plus aucun joueur n'est en vie
            if (nombreJoueurMort == partie.nombreEntite){
                fenetre = FIN;
            }
            //Si toutes les cases sont occupées par des objets solides
            if (nombreCaseObj(partie.terrain, VIDE) + nombreCaseObj(partie.terrain, FRUIT) + nombreCaseObj(partie.terrain, FRUIT_SPECIAL) == 0){
                fenetre = FIN;
            }
            #pragma endregion

            if (fenetre == FIN) {
                #pragma region GESTION FICHIER
                //Enregistrement fichier
                for (int i = 0; i < partie.nombreEntite; i++){
                    ecrire(CHEMIN_SCORE, partie.listEntite[i]->pseudo, partie.listEntite[i]->score, partie.graine);
                }
                #pragma endregion
            }
        }

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQ_AFFICHAGE);
    }
    #pragma endregion

    fermerController();

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(partie.terrain);
    //Les entités
    for (int i = 0; i < partie.nombreEntite; i++){
        free(partie.listEntite[i]);
    }

    return 0;
}