#include "view.h"
#include "controller.h"

pthread_mutex_t dmutex = PTHREAD_MUTEX_INITIALIZER;

int main(){

    #pragma region INITIALISATION

    #pragma region INITIALISATION TERRAIN
    int largeur = LARGEURTERRAIN; //permettra de faire des tailles variables par la suite
    int longueur = LONGEURTERRAIN; //permettra de faire des tailles variables par la suite

    int nombreMur = NOMBREMUR; //permettra de faire des nombres variables par la suite
    int nombreFruit = NOMBREFRUIT; //permettra de faire des nombres variables par la suite

    //initialisation terrain
    int** terrain = creerTerrain(largeur, longueur);
    initTerrain(terrain, largeur, longueur);

    //Initialisation du random
    srand(time(0));

    //Ajout des murs
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreMur, MUR);

    //Ajout des fruits
    ajouterAleatoireTerrain(terrain, largeur, longueur, nombreFruit, FRUIT);

    #pragma endregion

    #pragma region INITIALISATION ENTITE
    //Initialisation entites
    entite* listEntite[NOMBREMAXENTITE];

    int nombreEntite = 0;
    entite * joueur = creerEntite(3, 3, 0, '1', nombreEntite + 1, TAILLEINITIALESERPENT);
    listEntite[0] = joueur;
    nombreEntite++;

    terrain[5][5] = FRUIT;

    #pragma endregion
    
    //Score
    int score = 0;
    int evolutionScore = 0;

    int nombreFruitMange = 0;
    int nombreFruitSpecialMange = 0;

    int distanceParcouru = 0;

    //Detection clavier
    pthread_t clavier;

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
        sleep(1);
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
        if (caseDirection < 0){ //Si la case n'est pas "solide"
            //On pose une partie du corp du serpent
            terrain[joueur->posX][joueur->posY] = joueur->longueur - 1 + CLASSESERPENT * joueur->num;

            //Si on rencontre un fruit on agrandit le serpent et on fait évoluer le score
            if (caseDirection == FRUIT){
                evolutionScore = 10;
                nombreFruitMange++;
                ajouterClasseTerrain(terrain, largeur, longueur, joueur->num);
                joueur->longueur++;
            } else if (caseDirection == FRUITSPECIAL){
                evolutionScore = 50;
                nombreFruitSpecialMange++;
                ajouterClasseTerrain(terrain, largeur, longueur, joueur->num);
                joueur->longueur++;

                //On rajoute un fruit sur le terrain
                ajouterAleatoireTerrain(terrain, largeur, longueur, 1, FRUIT); 
            }


            //On déplace la tête
            deplacerDirection(joueur);

            distanceParcouru++;

        } else { //Si c'est un obstacle
            launched = false;
        }
        pthread_mutex_unlock(&dmutex);

        //On fait les tests sur le fruit spécial


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