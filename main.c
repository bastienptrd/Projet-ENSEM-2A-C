#include <stdio.h>

#include "view.h"
#include "controller.h"
#include "gestionFichier.h"

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    #pragma region INITIALISATION

    #pragma region INITIALISATION ENTITE
    //Initialisation entites
    entite* listEntite[NOMBREMAXENTITE];

    int nombreEntite = 0;
    entite * joueur = creerEntite(0, 0, DIRECTIONINITIALESERPENT, '1', nombreEntite + 1, TAILLEINITIALESERPENT);
    listEntite[0] = joueur;
    nombreEntite++;

    #pragma endregion

    #pragma region INITIALISATION TERRAIN
    int largeur = LARGEURTERRAIN; //permettra de faire des tailles variables par la suite
    int longueur = LONGEURTERRAIN; //permettra de faire des tailles variables par la suite

    int nombreMur = NOMBREMUR; //permettra de faire des nombres variables par la suite
    int nombreFruit = NOMBREFRUIT; //permettra de faire des nombres variables par la suite

    //initialisation terrain
    //int** terrain = creerTerrain(largeur, longueur);
    //initTerrain(terrain, largeur, longueur);
    int ** terrain = lireTerrain("plateau.txt", &largeur, &longueur);

    //Initialisation du random
    srand(time(0));

    //Ajout des murs
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreMur, OBSTACLE, NULL, NULL);

    //Ajouter le serpent (nécessite 2 case vide)
    int nombreEssai = 0;
    do {
        ajouterAleatoireTerrain(terrain, largeur, longueur, 1, VIDE, &(joueur->posX), &(joueur->posY));
        
        //Bloquer en cas de situation bloquante pour la position du seprent
        nombreEssai++;
        if (nombreEssai == NOMBREEESAIMAX){
            printf("ERREUR : pas assez d'espace pour le serpent dans cette direction");
            return 1;
        }

    } while (terrain[joueur->posX][joueur->posY] != VIDE || caseSuivante(joueur, terrain) != VIDE);

    //Ajout des fruits
    terrain[joueur->posX][joueur->posY] = FRUIT; //Empeche la génération en la position du joueur
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreFruit, FRUIT, NULL, NULL);
    terrain[joueur->posX][joueur->posY] = VIDE; //Empeche la génération en la position du joueur

    #pragma endregion
    
    //Fruit spécial
    time_t tempApparition = -1;
    int posFruitSpecialX;
    int posFruitSpecialY;

    //Score
    int score = 0;
    int evolutionScore = 0;

    int nombreFruitMange = 0;
    int nombreFruitSpecialMange = 0;

    int distanceParcouru = 0;

    //Detection clavier
    pthread_t clavier;

    char pseudo[20];
    scanf("%s", pseudo);

    //Condition de la boucle principale
    bool launched = true;

    #pragma endregion

    //Decompte vers le début
    for (int i = 3; i > 0; i--){
        clearAffichage();
        printfDemiLongueur(1, longueur, '=');
        printf(" %d ", i);
        printfDemiLongueur(1, longueur, '=');
        printf("\n");
        affichage(terrain, largeur, longueur, listEntite, nombreEntite);
        sleep(2);
    }
    
    //Recuperation des entrées clavier
    pthread_create(&clavier,NULL,lire_clavier,joueur);

    //Boucle principale
    while (launched){

        //On "déplace" le corps du serpent
        updateTerrain(terrain, largeur, longueur);

        //On verrouille le mutex pour eviter des changement de direction entre le teste et le déplacement
        pthread_mutex_lock(&dmutex);

        //On regarde la prochaine case atteinte par le serpent
        int caseDirection = caseSuivante(joueur, terrain);
        if (caseDirection < -1){ //Si la case n'est pas "solide"
            //On pose une partie du corp du serpent
            terrain[joueur->posX][joueur->posY] = joueur->longueur - 1 + CLASSESERPENT * joueur->num;

            //Si on rencontre un fruit on agrandit le serpent et on fait évoluer le score
            if (caseDirection == FRUIT){
                evolutionScore = 10;
                nombreFruitMange++;
                ajouterClasseTerrain(terrain, largeur, longueur, joueur->num);
                joueur->longueur++;

                //On rajoute un fruit sur le terrain
                ajouterAleatoireTerrain(terrain, largeur, longueur, 1, FRUIT, NULL, NULL); 
            } else if (caseDirection == FRUITSPECIAL){
                evolutionScore = 50;
                nombreFruitSpecialMange++;
                ajouterClasseTerrain(terrain, largeur, longueur, joueur->num);
                joueur->longueur++;
                tempApparition = -1;
            }


            //On déplace la tête
            deplacerDirection(joueur);

            distanceParcouru++;

        } else { //Si c'est un obstacle
            launched = false;
        }
        pthread_mutex_unlock(&dmutex);

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
        printf("Score : %d", score);
        if (evolutionScore > 0){
            printf(" + %d", evolutionScore);
        }
        printf("\n");
        affichage(terrain, largeur, longueur, listEntite, nombreEntite);

        //Evolution du score
        score += evolutionScore;
        evolutionScore = 0;

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQAFFICHAGE);
    }

    //Affichage du message de fin
    afficherFin(longueur, score, joueur->longueur, nombreFruitMange, nombreFruitSpecialMange, distanceParcouru);

    //Enregistrement fichier
    ecrire("scores", pseudo, score);

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