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

    int nombreTotalJoueur = 2;
    int graineDonner = -1;

    pthread_mutex_t listMutex[NOMBRE_MAX_ENTITE];
    listMutex[0] = dmutexjoueur1;
    listMutex[1] = dmutexjoueur2;

    #pragma region INITIALISATION ENTITE
    //Initialisation entites
    entite* listEntite[NOMBRE_MAX_ENTITE];

    int nombreEntite = 0;

    entite * joueur1 = creerEntite(nombreEntite + 1);
    listEntite[nombreEntite] = joueur1;
    nombreEntite++;

    entite * joueur2 = creerEntite(nombreEntite + 1);
    listEntite[nombreEntite] = joueur2;
    nombreEntite++;

    //Choix du nombre de joueurs
    if (nombreEntite != nombreTotalJoueur) {
        nombreEntite = nombreTotalJoueur;
    }

    
    #pragma endregion

    #pragma region INITIALISATION TERRAIN
    int largeur = LARGEUR_TERRAIN;
    int longueur = LONGEUR_TERRAIN;

    int nombreMur = NOMBRE_MUR;
    int nombreFruit = NOMBRE_FRUIT;

    bool fruitInfinie = FRUIT_INFINIE;
    int nombreFruitAjouter = NOMBRE_FRUIT_AJOUTER;

    int tailleAjouterFruit = TAILLE_AJOUTEE_FRUIT;
    int tailleAjouterFruitSpecial = TAILLE_AJOUTEE_FRUIT_SPECIAL;

    int scoreFruit = SCORE_FRUIT;
    int scoreFruitSpecial = SCORE_FRUIT_SPECIAL;

    // Initialisation terrain
    terrain * terrain = creerTerrain(largeur, longueur);
    initTerrain(terrain);
    /*terrain * terrain = malloc(sizeof(terrain));
    terrain->tableau = lireTerrain(CHEMIN_TERRAIN, &largeur, &longueur);
    terrain->largeur = largeur;
    terrain->longueur = longueur;*/

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
            
            //Retourner erreur en cas de situation bloquante pour la position du seprent
            nombreEssai++;
            if (nombreEssai == NOMBRE_ESSAI_MAX){
                printf("ERREUR : pas assez d'espace pour le serpent");
                return 1;
            }

        } while (terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] != VIDE || caseSuivante(listEntite[i], terrain->tableau) != VIDE);
        
        //On place le corps du serpents sur le terrain
        terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] = listEntite[i]->longueur + CLASSE_SERPENT * listEntite[i]->num; 
    }
    
    //Ajout des fruits
    ajouterAleatoireTerrain(terrain, nombreFruit, FRUIT, NULL, NULL);
    
    #pragma endregion
    
    //Condition de la boucle principale
    bool launched = true;

    //Création argument thread
    entites ensembleJoueur;
    for (int i = 0; i < nombreEntite; i++){
        ensembleJoueur.listEntite[i] = listEntite[i];
    }
    ensembleJoueur.nombreEntite = nombreEntite;
    ensembleJoueur.pass = false;
    ensembleJoueur.launched = &launched;

    //Permet d'arreter la boucle au besoin
    int nombreJoueurEnVie = nombreEntite;
    //Variables fruit spécial
    time_t tempApparition = -1;
    int posFruitSpecialX;
    int posFruitSpecialY;

    #pragma endregion

    initWindow();
    
    afficherTitre(terrain->longueur);

    //Entrer pseudo des joueurs
    for (int i = 0; i < nombreEntite; i++){
        lirePseudo(listEntite[i]->pseudo);
    }
    
    initialiserController(&ensembleJoueur);

    //Selectionne une direction
    for (int i = 0; i < nombreEntite; i++){
        listEntite[i]->direction = DIRECTION_DEFAULT_SERPENT;
    }

    //Autorise les joueurs à changer librement de direction avant le début
    ensembleJoueur.pass = true;

    gererEvent(&ensembleJoueur);
    //Decompte vers le début
    afficherDecompte(terrain, listEntite, nombreEntite);

    //Retire authorisation du changement libre
    ensembleJoueur.pass = false;

    //Boucle principale
    while (launched){

        gererEvent(&ensembleJoueur);

        #pragma region DEPLACEMENT SERPENT
        //Déplacement des serpents
        for (int i = 0; i < nombreEntite; i++){
            //Si le joueur est mort, on passe au suivant
            if (listEntite[i]->enVie == 0){
                continue;
            }

            //On verrouille le mutex pour eviter des changements de direction entre le teste et le déplacement
            pthread_mutex_lock(listMutex + i);

            //On regarde la prochaine case atteinte par le serpent
            int caseDirection = caseSuivante(listEntite[i], terrain->tableau);

            //Si c'est un obstacle
            if (caseDirection > CLASSE_SERPENT || caseDirection == MUR || caseDirection == OBSTACLE){
                listEntite[i]->enVie = false;
                nombreJoueurEnVie--;
                pthread_mutex_unlock(listMutex + i); 
                continue;
            }

            //Si la case n'est pas "solide"
            //On "déplace" le corps du serpent
            ajouterClasseTerrain(terrain, listEntite[i]->num, -1);

            //Si on rencontre un fruit, on agrandit le serpent et on fait évoluer le score
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
            } else if (caseDirection == FRUIT_SPECIAL){
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
            terrain->tableau[listEntite[i]->posX][listEntite[i]->posY] = listEntite[i]->longueur + CLASSE_SERPENT * listEntite[i]->num;

            listEntite[i]->distanceParcourue++;
        }
        #pragma endregion

        #pragma region FRUIT SPECIAL
        //On fait les tests sur le fruit spécial
        if (tempApparition == -1 && rand() % FREQ_APPARITION_FRUIT_SPECIAL * FREQ_DEPLACEMENT == 0){
            tempApparition = time(NULL);
            ajouterAleatoireTerrain(terrain, 1, FRUIT_SPECIAL, &posFruitSpecialX, &posFruitSpecialY);
        } else if (tempApparition != -1 && difftime(time(NULL), tempApparition) > TEMP_APPARITION_FRUIT_SPECIAL){
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
        //Si on a mangé tous les fruits
        if (nombreCaseObj(terrain, FRUIT) == 0){
            launched = false;
        }
        //Si plus aucun joueur n'est en vie
        if (nombreJoueurEnVie == 0){
            launched = false;
        }
        //Si toutes les cases sont occupées par des objets solides
        if (nombreCaseObj(terrain, VIDE) + nombreCaseObj(terrain, FRUIT) + nombreCaseObj(terrain, FRUIT_SPECIAL) == 0){
            launched = false;
        }
        #pragma endregion

        //Temps de pause pour soulager le cpu
        usleep(1000000 / FREQ_DEPLACEMENT);
    }

    #pragma region AFFICHAGE FIN

    //Affichage vainqueur
    afficherFin(terrain->longueur, listEntite, nombreEntite);

    #pragma endregion

    #pragma region GESTION FICHIER
    //Enregistrement fichier
    for (int i = 0; i < nombreEntite; i++){
        ecrire(CHEMIN_SCORE, listEntite[i]->pseudo, listEntite[i]->score, graine);    
    }
    #pragma endregion

    fermerController();

    //On rend la mémoire allouée
    //Le terrain
    freeTerrain(terrain);
    //Les entités
    for (int i = 0; i < nombreEntite; i++){
        free(listEntite[i]);
    }

    return 0;
}