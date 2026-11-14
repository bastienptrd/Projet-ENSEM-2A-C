#include "../../include/partie.h"
#include "../../include/gestionFichier.h"

#include <stdio.h>
#include <stdlib.h>

donneePartie * partieVide() {
    donneePartie * partie = malloc(sizeof(donneePartie));

    //Condition de la boucle principale
    partie->launched = true;

    partie->nombreEntite = NOMBRE_MAX_ENTITE;
    for (int i = 0; i < NOMBRE_MAX_ENTITE; i++) {
        partie->listEntite[i] = NULL;
    }
    partie->terrain = NULL;

    partie->indexLettrePseudo = 0;
    partie->indexEntitePseudo = 0;

    partie->fruitInfinie = FRUIT_INFINIE;
    partie->nombreFruitAjouter = NOMBRE_FRUIT_AJOUTER;
    partie->tailleAjouterFruit = TAILLE_AJOUTEE_FRUIT;
    partie->tailleAjouterFruitSpecial = TAILLE_AJOUTEE_FRUIT_SPECIAL;
    partie->scoreFruit = SCORE_FRUIT;
    partie->scoreFruitSpecial = SCORE_FRUIT_SPECIAL;

    return partie;
}

void initialiserPartieEntite(donneePartie * partie) {
    //Initialisation entites
    int indexEntite = 0;
    if (partie->listEntite[indexEntite] == NULL) {
        partie->listEntite[indexEntite] = creerEntite(indexEntite + 1);
    } else {
        initialiserEntite(partie->listEntite[indexEntite]);
    }
    indexEntite++;

    if (partie->nombreEntite == indexEntite) {
        return;
    }

    if (partie->listEntite[indexEntite] == NULL) {
        partie->listEntite[indexEntite] = creerEntite(indexEntite + 1);
    } else {
        initialiserEntite(partie->listEntite[indexEntite]);
    }
}

void initialiserPartieTerrain(donneePartie * partie) {
    int largeur = LARGEUR_TERRAIN;
    int longueur = LONGEUR_TERRAIN;

    int nombreMur = NOMBRE_MUR;
    int nombreFruit = NOMBRE_FRUIT;

    // Initialisation terrain
    if (partie->terrain == NULL) {
        partie->terrain = creerTerrain(largeur, longueur);
    }
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
    partie->tempApparitionFruitSpecial = -1;
}

void incrementerScore(donneePartie * partie) {
    //Calculs des scores du passage precédent (laisse le temps à l'affichage)
    for (int i = 0; i < partie->nombreEntite; i++){
        partie->listEntite[i]->score += partie->listEntite[i]->evolutionScore;
        partie->listEntite[i]->evolutionScore = 0;
    }
}

void deplacementSerpents(donneePartie * partie, pthread_mutex_t listMutex[NOMBRE_MAX_ENTITE]) {
    for (int i = 0; i < partie->nombreEntite; i++){
        //Si le joueur est mort, on passe au suivant
        if (partie->listEntite[i]->enVie == 0){
            continue;
        }

        //On verrouille le mutex pour eviter des changements de direction entre le teste et le déplacement
        pthread_mutex_lock(listMutex + i);

        //On regarde la prochaine case atteinte par le serpent

        int caseDirection = caseSuivante(partie->listEntite[i], partie->terrain);

        //Si c'est un obstacle
        if (caseDirection > CLASSE_SERPENT || caseDirection == MUR || caseDirection == OBSTACLE){
            partie->listEntite[i]->enVie = false;
            pthread_mutex_unlock(listMutex + i);
            continue;
        }

        //Si la case n'est pas "solide"
        //On "déplace" le corps du serpent
        ajouterClasseTerrain(partie->terrain, partie->listEntite[i]->num, -1);

        //Si on rencontre un fruit, on agrandit le serpent et on fait évoluer le score
        if (caseDirection == FRUIT){
            ajouterClasseTerrain(partie->terrain, partie->listEntite[i]->num, partie->tailleAjouterFruit);
            partie->listEntite[i]->longueur++;
            partie->listEntite[i]->evolutionScore = partie->scoreFruit;
            partie->listEntite[i]->nombreFruitMange++;

            if (partie->fruitInfinie){
                //On rajoute un fruit sur le terrain
                ajouterAleatoireTerrain(partie->terrain, partie->nombreFruitAjouter, FRUIT, NULL, NULL);
            }
        } else if (caseDirection == FRUIT_SPECIAL){
            ajouterClasseTerrain(partie->terrain, partie->listEntite[i]->num, partie->tailleAjouterFruitSpecial);
            partie->listEntite[i]->longueur++;
            partie->listEntite[i]->evolutionScore = partie->scoreFruitSpecial;
            partie->listEntite[i]->nombreFruitSpecialMange++;
            partie->tempApparitionFruitSpecial = -1;
        }

        //On déplace la tête
        deplacerDirection(partie->listEntite[i]);

        pthread_mutex_unlock(listMutex + i);

        //On pose une partie du corp du serpent
        partie->terrain->tableau[partie->listEntite[i]->posX][partie->listEntite[i]->posY] = partie->listEntite[i]->longueur + CLASSE_SERPENT * partie->listEntite[i]->num;

        partie->listEntite[i]->distanceParcourue++;
    }
}

void updateFruitSpecial(donneePartie * partie) {
    //On fait les tests sur le fruit spécial
    if (partie->tempApparitionFruitSpecial == -1 && rand() % FREQ_APPARITION_FRUIT_SPECIAL * FREQ_AFFICHAGE == 0){
        partie->tempApparitionFruitSpecial = time(NULL);
        ajouterAleatoireTerrain(partie->terrain, 1, FRUIT_SPECIAL, &partie->posFruitSpecialX, &partie->posFruitSpecialY);
    } else if (partie->tempApparitionFruitSpecial != -1 && difftime(time(NULL), partie->tempApparitionFruitSpecial) > TEMP_APPARITION_FRUIT_SPECIAL){
        partie->terrain->tableau[partie->posFruitSpecialX][partie->posFruitSpecialY] = VIDE;
        partie->tempApparitionFruitSpecial = -1;
    }
}

void testFin(donneePartie * partie, int * fenetre) {
    int nombreJoueurEnVie = 0;
    for (int i = 0; i < partie->nombreEntite; i++) {
        if (partie->listEntite[i]->enVie) {
            nombreJoueurEnVie++;
        }
    }
    //Test fin de partie
    //Si on a mangé tous les fruits
    if (nombreCaseObj(partie->terrain, FRUIT) == 0){
        *fenetre = FIN;
    }
    //Si plus aucun joueur n'est en vie
    if (nombreJoueurEnVie == 0){
        *fenetre = FIN;
    }
    //Si toutes les cases sont occupées par des objets solides
    if (nombreCaseObj(partie->terrain, VIDE) + nombreCaseObj(partie->terrain, FRUIT) + nombreCaseObj(partie->terrain, FRUIT_SPECIAL) == 0){
        *fenetre = FIN;
    }
}

void enregistrerPartie(donneePartie * partie){
    //Enregistrement fichier
    for (int i = 0; i < partie->nombreEntite; i++){
        ecrire(CHEMIN_SCORE, partie->listEntite[i]->pseudo, partie->listEntite[i]->score, partie->graine);
    }
}

void libererPartie(donneePartie * partie) {
    if (partie == NULL) {
        return;
    }
    //Le terrain
    freeTerrain(partie->terrain);
    //Les entités
    for (int i = 0; i < NOMBRE_MAX_ENTITE; i++){
        if (partie->listEntite[i] == NULL) {
            continue;
        }
        free(partie->listEntite[i]);
    }
    free(partie);
}
