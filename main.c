#include <stdio.h>

#include "view.h"
#include "controller.h"
#include "gestionFichier.h"
#include "utilitaire.h"

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

    entite * joueur1 = creerEntite(nombreEntite + 1);
    listEntite[nombreEntite] = joueur1;
    nombreEntite++;

    entite * joueur2 = creerEntite(nombreEntite + 1);
    listEntite[nombreEntite] = joueur2;
    nombreEntite++;

    //Choix du nombre de joueur
    nombreEntite = nombreTotalJoueur;
    
    #pragma endregion

    #pragma region INITIALISATION TERRAIN
    int largeur = LARGEURTERRAIN;
    int longueur = LONGEURTERRAIN;

    int nombreMur = NOMBREMUR;
    int nombreFruit = NOMBREFRUIT;

    bool fruitInfinie = FRUITINFINIE;
    int nombreFruitAjouter = NOMBREFRUITAJOUTER;

    int tailleAjouterFruit = TAILLEAJOUTEEFRUIT;
    int tailleAjouterFruitSpecial = TAILLEAJOUTEEFRUITSPECIAL;

    int scoreFruit = SCOREFRUIT;
    int scoreFruitSpecial = SCOREFRUITSPECIAL;

    //initialisation terrain
    //terrain * terrain = creerTerrain(largeur, longueur);
    //initTerrain(terrain);
    terrain * terrain = malloc(sizeof(terrain));
    terrain->tableau = lireTerrain(CHEMINTERRAIN, &largeur, &longueur);
    terrain->largeur = largeur;
    terrain->longueur = longueur;

    //Initialisation du random
    int graine = time(NULL);
    if (graineDonner != -1){
        graine = 0;
    }
    srand(graine);

    //Ajout des murs
    ajouterAleatoireTerrain(terrain, nombreMur, OBSTACLE, NULL, NULL);

    //Ajout des serpents
    for (int i = 0; i < nombreEntite; i++){
        //Ajouter le serpent (nécessite 2 case vide)
        int nombreEssai = 0;
        do {
            ajouterAleatoireTerrain(terrain, 1, VIDE, &(listEntite[i]->posX), &(listEntite[i]->posY));
            
            //Bloquer en cas de situation bloquante pour la position du seprent
            nombreEssai++;
            if (nombreEssai == NOMBREEESSAIMAX){
                printf("ERREUR : pas assez d'espace pour le serpent dans cette direction");
                return 1;
            }

        } while (terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] != VIDE || caseSuivante(listEntite[i], terrain->tableau) != VIDE);
        
        terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] = listEntite[i]->longueur + CLASSESERPENT * listEntite[i]->num; 
    }
    
    //Ajout des fruits
    ajouterAleatoireTerrain(terrain, nombreFruit, FRUIT, NULL, NULL);
    
    #pragma endregion
    
    //Variables fruit spécial
    time_t tempApparition = -1;
    int posFruitSpecialX;
    int posFruitSpecialY;

    //Entrer pseudo des joueurs
    for (int i = 0; i < nombreEntite; i++){
        printf("Entrez pseudo joueur %d :", listEntite[i]->num);
        scanf("%s", listEntite[i]->pseudo);
    }
    
    //Création argument thread
    entites ensembleJoueur;
    for (int i = 0; i < nombreEntite; i++){
        ensembleJoueur.listEntite[i] = listEntite[i];
    }
    ensembleJoueur.nombreEntite = nombreEntite;

    //Permet d'arreter la boucle au besoin
    int nombreJoueurEnVie = nombreEntite;

    //Condition de la boucle principale
    bool launched = true;

    #pragma endregion
    
    initialiserTerminal();

    //Detection clavier
    pthread_t clavier;

    //Recuperation des entrées clavier
    pthread_create(&clavier,NULL,lireClavier,&ensembleJoueur);

    //Selectionne aucune direction de sorte à ce que le joueur puisse choisir nimporte laquelle
    for (int i = 0; i < nombreEntite; i++){
        listEntite[i]->direction = DIRECTIONINITIALESERPENT;
    }

    //Decompte vers le début
    afficherDecompte(terrain, listEntite, nombreEntite);

    //Si le joueur n'a pas choisi de direction on remet alors celle de default
    for (int i = 0; i < nombreEntite; i++){
        if (listEntite[i]->direction == DIRECTIONINITIALESERPENT){
            listEntite[i]->direction = DIRECTIONDEFAULTSERPENT;
        }
    }

    //Boucle principale
    while (launched){

        #pragma region DEPLACEMENT SERPENT
        //Déplacement des serpents
        for (int i = 0; i < nombreEntite; i++){
            //Si le joueur est mort on passe au suivant
            if (listEntite[i]->enVie == 0){
                continue;
            }

            //On verrouille le mutex pour eviter des changement de direction entre le teste et le déplacement
            pthread_mutex_lock(listMutex + i);

            //On regarde la prochaine case atteinte par le serpent
            int caseDirection = caseSuivante(listEntite[i], terrain->tableau);

            //Si c'est un obstacle
            if (caseDirection > CLASSESERPENT || caseDirection == MUR || caseDirection == OBSTACLE){
                listEntite[i]->enVie = false;
                nombreJoueurEnVie--;
                pthread_mutex_unlock(listMutex + i); 
                continue;
            }

            //Si la case n'est pas "solide"
            //On "déplace" le corps du serpent
            ajouterClasseTerrain(terrain, listEntite[i]->num, -1);

            //Si on rencontre un fruit on agrandit le serpent et on fait évoluer le score
            if (caseDirection == FRUIT){
                listEntite[i]->evolutionScore = scoreFruit;
                listEntite[i]->nombreFruitMange++;
                ajouterClasseTerrain(terrain, listEntite[i]->num, tailleAjouterFruit);
                listEntite[i]->longueur++;

                if (fruitInfinie){
                    //On rajoute un fruit sur le terrain
                    ajouterAleatoireTerrain(terrain, nombreFruitAjouter, FRUIT, NULL, NULL);
                    nombreFruit++;
                }
            } else if (caseDirection == FRUITSPECIAL){
                listEntite[i]->evolutionScore = scoreFruitSpecial;
                listEntite[i]->nombreFruitSpecialMange++;
                ajouterClasseTerrain(terrain, listEntite[i]->num, tailleAjouterFruitSpecial);
                listEntite[i]->longueur++;
                tempApparition = -1;
            }

            //On déplace la tête
            deplacerDirection(listEntite[i]);

            pthread_mutex_unlock(listMutex + i);

            //On pose une partie du corp du serpent
            terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] = listEntite[i]->longueur + CLASSESERPENT * listEntite[i]->num;

            listEntite[i]->distanceParcourue++;
        }
        #pragma endregion

        #pragma region FRUIT SPECIAL
        //On fait les tests sur le fruit spécial
        if (tempApparition == -1 && rand() % FREQAPPARITIONFRUITSPECIAL * FREQDEPLACEMENT == 0){
            tempApparition = time(NULL);
            ajouterAleatoireTerrain(terrain, 1, FRUITSPECIAL, &posFruitSpecialX, &posFruitSpecialY);
        } else if (tempApparition != -1 && difftime(time(NULL), tempApparition) > TEMPAPPARITIONFRUITSPECIAL){
            terrain->tableau[posFruitSpecialX][posFruitSpecialY] = VIDE;
            tempApparition = -1;
        }
        #pragma endregion

        #pragma region AFFICHAGE
        afficherPartie(terrain, listEntite, nombreEntite);
        #pragma endregion

        #pragma region SCORE
        //Calculs des scores
        for (int i = 0; i < nombreEntite; i++){
            listEntite[i]->score += listEntite[i]->evolutionScore;
            listEntite[i]->evolutionScore = 0;
        }
        #pragma endregion

        #pragma region TESTS FIN
        //Test fin de partie
        //Si on a mangé tout les fruits
        if (nombreCaseObj(terrain, FRUIT) == 0){
            launched = false;
        }
        //Si plus aucun joueur est en vie
        if (nombreJoueurEnVie == 0){
            launched = false;
        }
        //Si toutes les cases sont occupées par des objets solides
        if (nombreCaseObj(terrain, VIDE) + nombreCaseObj(terrain, FRUIT) + nombreCaseObj(terrain, FRUITSPECIAL) == 0){
            launched = false;
        }
        #pragma endregion

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQDEPLACEMENT);
    }

    #pragma region AFFICHAGE FIN
    //Affichage vainqueur
    afficherFin(terrain->longueur, listEntite, nombreEntite);
    #pragma endregion

    #pragma region GESTION FICHIER
    //Enregistrement fichier
    for (int i = 0; i < nombreEntite; i++){
        ecrire(CHEMINSCORE, listEntite[i]->pseudo, listEntite[i]->score, graine);    
    }
    #pragma endregion

    //On arrete le thread clavier
    pthread_cancel(clavier);
    pthread_join(clavier, NULL);

    restaurerTerminal();

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(terrain);
    //Les entités
    for (int i = 0; i < nombreEntite; i++){
        free(listEntite[i]);
    }

    return 0;
}