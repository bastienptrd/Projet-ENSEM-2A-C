#include <stdio.h>

#include "view.h"
#include "controller.h"
#include "gestionFichier.h"

pthread_mutex_t dmutexjoueur1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dmutexjoueur2 = PTHREAD_MUTEX_INITIALIZER;

int main(){
    #pragma region INITIALISATION

    int nombreTotalJoueur = 2;
    int graineDonner = -1;

    pthread_mutex_t listMutex[2];
    listMutex[0] = dmutexjoueur1;
    listMutex[1] = dmutexjoueur2;

    #pragma region INITIALISATION ENTITE
    //Initialisation entites
    entite* listEntite[NOMBREMAXENTITE];

    int nombreEntite = 0;

    entite * joueur1 = creerEntite(0, 0, DIRECTIONINITIALESERPENT, '1', nombreEntite + 1, TAILLEINITIALESERPENT);
    listEntite[nombreEntite] = joueur1;
    nombreEntite++;

    entite * joueur2 = creerEntite(0, 0, DIRECTIONINITIALESERPENT, '2', nombreEntite + 1, TAILLEINITIALESERPENT);
    listEntite[nombreEntite] = joueur2;
    nombreEntite++;

    //Choix du nombre de joueur
    nombreEntite = nombreTotalJoueur;
    
    #pragma endregion

    #pragma region INITIALISATION TERRAIN
    int largeur = LARGEURTERRAIN; //permettra de faire des tailles variables par la suite
    int longueur = LONGEURTERRAIN; //permettra de faire des tailles variables par la suite

    int nombreMur = NOMBREMUR; //permettra de faire des nombres variables par la suite
    int nombreFruit = NOMBREFRUIT; //permettra de faire des nombres variables par la suite

    bool fruitInfinie = FRUITINFINIE;
    int nombreFruitAjouter = NOMBREFRUITAJOUTER;

    int tailleAjouterFruit = TAILLEAJOUTEEFRUIT;
    int tailleAjouterFruitSpecial = TAILLEAJOUTEEFRUITSPECIAL;

    int scoreFruit = SCOREFRUIT;
    int scoreFruitSpecial = SCOREFRUITSPECIAL;

    //initialisation terrain
    //int** terrain = creerTerrain(largeur, longueur);
    //initTerrain(terrain, largeur, longueur);
    int ** terrain = lireTerrain(CHEMINTERRAIN, &largeur, &longueur);

    //Initialisation du random
    int graine = time(NULL);
    if (graineDonner != -1){
        graine = 0;
    }
    srand(graine);

    //Ajout des murs
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreMur, OBSTACLE, NULL, NULL);

    for (int i = 0; i < nombreEntite; i++){
        //Ajouter le serpent (nécessite 2 case vide)
        int nombreEssai = 0;
        do {
            ajouterAleatoireTerrain(terrain, largeur, longueur, 1, VIDE, &(listEntite[i]->posX), &(listEntite[i]->posY));
            
            //Bloquer en cas de situation bloquante pour la position du seprent
            nombreEssai++;
            if (nombreEssai == NOMBREEESSAIMAX){
                printf("ERREUR : pas assez d'espace pour le serpent dans cette direction");
                return 1;
            }

        } while (terrain[listEntite[i]->posX][listEntite[i]->posY] != VIDE || caseSuivante(listEntite[i], terrain) != VIDE);
        
        terrain[listEntite[i]->posX][listEntite[i]->posY] = FRUIT; //Empeche la génération en la position du joueur
    }
    
    //Ajout des fruits
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreFruit, FRUIT, NULL, NULL);
    
    for (int i = 0; i < nombreEntite; i++){
        terrain[listEntite[i]->posX][listEntite[i]->posY] = VIDE; //Empeche la génération en la position du joueur
    }
    #pragma endregion
    
    //Fruit spécial
    time_t tempApparition = -1;
    int posFruitSpecialX;
    int posFruitSpecialY;

    //Score
    int score[NOMBREMAXENTITE] = { 0 };
    int evolutionScore[NOMBREMAXENTITE] = { 0 };

    int nombreFruitMange[NOMBREMAXENTITE] = { 0 };
    int nombreFruitSpecialMange[NOMBREMAXENTITE] = { 0 };

    int distanceParcouru[NOMBREMAXENTITE] = { 0 };

    char pseudo[NOMBREMAXENTITE][20];
    for (int i = 0; i < nombreEntite; i++){
        printf("Entrez pseudo joueur %d :", listEntite[i]->num);
        scanf("%19s", pseudo[i]);
    }
    
    //Création argument thread
    entites ensembleJoueur;
    for (int i = 0; i < nombreEntite; i++){
        ensembleJoueur.listEntite[i] = listEntite[i];
    }
    ensembleJoueur.nombreEntite = nombreEntite;

    //Permet d'arretet la boucle au besoin
    int nombreJoueurEnVie = nombreEntite;

    //Condition de la boucle principale
    bool launched = true;

    #pragma endregion

    
    initialiserTerminal();

    //Detection clavier
    pthread_t clavier;

    //Recuperation des entrées clavier
    pthread_create(&clavier,NULL,lireClavier,&ensembleJoueur);

    //Decompte vers le début
    for (int i = 3; i > 0; i--){
        clearAffichage();
        char message[5];
        sprintf(message, "%d", i);
        printfLongueur(message, longueur, '=');
        affichage(terrain, largeur, longueur, listEntite, nombreEntite);
        sleep(2);
    }

    //Boucle principale
    while (launched){

        for (int i = 0; i < nombreEntite; i++){
            //Si le joueur est mort on passe au suivant
            if (listEntite[i]->enVie == 0){
                continue;
            }

            //On verrouille le mutex pour eviter des changement de direction entre le teste et le déplacement
            pthread_mutex_lock(listMutex + i);

            //On regarde la prochaine case atteinte par le serpent
            int caseDirection = caseSuivante(listEntite[i], terrain);
            if (caseDirection < -1){ //Si la case n'est pas "solide"
                //On "déplace" le corps du serpent
                ajouterClasseTerrain(terrain, largeur, longueur, listEntite[i]->num, -1);

                //On pose une partie du corp du serpent
                terrain[listEntite[i]->posX][listEntite[i]->posY] = listEntite[i]->longueur - 1 + CLASSESERPENT * listEntite[i]->num;

                //Si on rencontre un fruit on agrandit le serpent et on fait évoluer le score
                if (caseDirection == FRUIT){
                    evolutionScore[i] = scoreFruit;
                    nombreFruitMange[i]++;
                    ajouterClasseTerrain(terrain, largeur, longueur, listEntite[i]->num, tailleAjouterFruit);
                    listEntite[i]->longueur++;
                    nombreFruit--;

                    if (fruitInfinie){
                        //On rajoute un fruit sur le terrain
                        ajouterAleatoireTerrain(terrain, largeur, longueur, nombreFruitAjouter, FRUIT, NULL, NULL); 
                        nombreFruit++;
                    }
                } else if (caseDirection == FRUITSPECIAL){
                    evolutionScore[i] = scoreFruitSpecial;
                    nombreFruitSpecialMange[i]++;
                    ajouterClasseTerrain(terrain, largeur, longueur, listEntite[i]->num, tailleAjouterFruitSpecial);
                    listEntite[i]->longueur++;
                    tempApparition = -1;
                }

                //On déplace la tête
                deplacerDirection(listEntite[i]);

                distanceParcouru[i]++;

            } else { //Si c'est un obstacle
                listEntite[i]->enVie = false;
                nombreJoueurEnVie--;
            }
            pthread_mutex_unlock(listMutex + i); 
        }


        //On fait les tests sur le fruit spécial
        if (tempApparition == -1 && rand() % FREQAPPARITIONFRUITSPECIAL * FREQAFFICHAGE == 0){
            tempApparition = time(NULL);
            ajouterAleatoireTerrain(terrain, largeur, longueur, 1, FRUITSPECIAL, &posFruitSpecialX, &posFruitSpecialY);
        } else if (tempApparition != -1 && difftime(time(NULL), tempApparition) > TEMPAPPARITIONFRUITSPECIAL){
            terrain[posFruitSpecialX][posFruitSpecialY] = VIDE;
            tempApparition = -1;
        }

        //Faire l'affichage
        clearAffichage();
        
        for (int i = 0; i < nombreEntite; i++){
            printf("Score %s : %d", pseudo[i], score[i]);
            if (evolutionScore[i] > 0){
                printf(" + %d", evolutionScore[i]);
            }
            printf("\n");
        }
        affichage(terrain, largeur, longueur, listEntite, nombreEntite);

        //Evolution du score
        for (int i = 0; i < nombreEntite; i++){
            score[i] += evolutionScore[i];
            evolutionScore[i] = 0;
        }
        
        //Test fin de partie
        if (nombreFruit <= 0){
            launched = false;
        }
        if (nombreJoueurEnVie == 0){
            launched = false;
        }
        if (nombreCaseObj(terrain, largeur, longueur, VIDE) == 0){
            launched = false;
        }


        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQAFFICHAGE);
    }

    //Affichage vainqueur
    if (nombreJoueurEnVie == 0 && nombreEntite >= 2){
        printfLongueur("Partie nulle", longueur, '=');
    } else if (nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = score[0];
        for (int i = 1; i < nombreEntite; i++){
            if (score[i] > scoreMax){
                scoreMax = score[i];
                vainqueur = listEntite[i]->num;
            }
        }
        char message[30];
        sprintf(message, "%s gagne", pseudo[vainqueur - 1]);
        printfLongueur(message, longueur, '=');
    } else if (nombreJoueurEnVie == 0){
        printfLongueur("Défaite", longueur, '=');
    } else {
        printfLongueur("Victoire", longueur, '=');
    }

    //Affichage du message de fin
    for (int i = 0; i < nombreEntite; i++){
        printf("Joueur : %s\n", pseudo[i]);
        afficherFin(score[i], listEntite[i]->longueur, nombreFruitMange[i], nombreFruitSpecialMange[i], distanceParcouru[i]);
    }

    //Enregistrement fichier
    for (int i = 0; i < nombreEntite; i++){
        ecrire(CHEMINSCORE, pseudo[i], score[i], graine);    
    }

    //On arrete le thread clavier
    pthread_cancel(clavier);
    pthread_join(clavier, NULL);

    restaurerTerminal();

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(terrain, largeur);
    //Les entités
    for (int i = 0; i < nombreEntite; i++){
        free(listEntite[i]);
    }

    return 0;
}