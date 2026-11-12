#include <stdio.h>
#include <unistd.h>
#include "../include/configSDL.h"
#include "../include/view.h"
#include "../include/utilitaire.h"


SDL_Window *window;
SDL_Renderer *renderer;
SDL_Color couleurs[NOMBRE_MAX_ENTITE];
SDL_Color vert = {.r = 0, .g = 255, .b = 0, .a = 255};
SDL_Color magenta = {.r = 255, .g = 0, .b = 255, .a = 255};


void initWindow(){    
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }
    window = SDL_CreateWindow(TITRE_FENETRE, TAILLE_FENETRE_X, TAILLE_FENETRE_Y, SDL_WINDOW_RESIZABLE);
    if (window == NULL){
        SDL_Log("Erreur création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL){
        SDL_Log("Erreur création renderer : %s", SDL_GetError());
        detruireWindow();
        return;
    }

    couleurs[0] = vert;
    couleurs[1] = magenta;
}

static void affichageTerrain(terrain * terrain, entites * entites){
    // On remplit l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On choisit la couleur bleue
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    int windowSizeX;
    int windowSizeY;
    SDL_GetWindowSize(window, &windowSizeX, &windowSizeY);

    float tailleCase = min(TAILLE_P_PLATEAU_JEU_X * windowSizeX / (float)terrain->longueur, TAILLE_P_PLATEAU_JEU_Y * windowSizeY / (float)terrain->largeur);

    SDL_FRect caseRect = {0, 0, tailleCase, tailleCase};

    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){
            
            caseRect.x = tailleCase * j + POSITION_P_PLATEAU_JEU_X * windowSizeX;
            caseRect.y = tailleCase * i + POSITION_P_PLATEAU_JEU_Y * windowSizeY;

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < entites->nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (entites->listEntite[k]->posX == i && entites->listEntite[k]->posY == j){
                    SDL_SetRenderDrawColor(renderer, couleurs[k].r, couleurs[k].g, couleurs[k].b, couleurs[k].a);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner, on passe à la suivante
            if (!caseVide){
                
            } else if (terrain->tableau[i][j] == VIDE){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            } else if (terrain->tableau[i][j] == MUR){
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            } else if (terrain->tableau[i][j] == OBSTACLE){
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            } else if (terrain->tableau[i][j] == FRUIT){
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            } else if (terrain->tableau[i][j] == FRUIT_SPECIAL){
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            } else if (terrain->tableau[i][j] > 0) {
                int classe = terrain->tableau[i][j] / CLASSE_SERPENT - 1;
                SDL_SetRenderDrawColor(renderer, couleurs[classe].r, couleurs[classe].g, couleurs[classe].b, couleurs[classe].a);
            }

            SDL_RenderFillRect(renderer, &caseRect);
        }
    }

}

void afficherTitre(int longueur){
    UNUSED(longueur);

    // On remplit l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On choisit la couleur blanche
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_SetRenderScale(renderer, 12.0f, 12.0f);
    SDL_RenderDebugText(renderer, 10, 10, "SNAKE");
    SDL_SetRenderScale(renderer, 3.0f, 3.0f);
    SDL_RenderDebugText(renderer, 10, 100, "Appuyez sur espace");
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);

    // On met à jour la fenêtre
    SDL_RenderPresent(renderer);
}

void afficherPseudo(entites * entites, int indexLettrePseudo, int numEntite) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_FRect rect;
    rect.h = 8;
    rect.w = 1;
    rect.x = 10 + 8 * indexLettrePseudo;
    rect.y = 108 + 30 * numEntite + 1;

    // On choisit la couleur blanche
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_SetRenderScale(renderer, 12.0f, 12.0f);
    SDL_RenderDebugText(renderer, 10, 10, "SNAKE");
    SDL_SetRenderScale(renderer, 3.0f, 3.0f);

    SDL_RenderFillRect(renderer, &rect);
    for (int j = 0; j < entites->nombreEntite; j++) {
        SDL_RenderDebugText(renderer, 10, 100 + 30 * j, "Entrez pseudo : ");
        SDL_RenderDebugText(renderer, 10, 110 + 30 * j, entites->listEntite[j]->pseudo);
    }

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_RenderPresent(renderer);
}

void afficherDecompte(terrain * terrain, entites * entites, time_t tempsDebut){
    affichageTerrain(terrain, entites);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int i = difftime(time(NULL), tempsDebut) / TEMPS_CHIFFRES_DECOMPTE;

    char message[20];
    sprintf(message, " %d ", NOMBRE_DECOMPTE - i);
    SDL_SetRenderScale(renderer, 3.0f, 3.0f);
    SDL_RenderDebugText(renderer, 10, 10, message);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_RenderPresent(renderer);
}
void afficherPartie(terrain * terrain, entites * entites){
    affichageTerrain(terrain, entites);

    for (int i = 0; i < entites->nombreEntite; i++){
        char message[100];
        if (entites->listEntite[i]->evolutionScore == 0) {
            sprintf(message, "Score %s : %d", entites->listEntite[i]->pseudo, entites->listEntite[i]->score);
        } else {
            sprintf(message, "Score %s : %d + %d", entites->listEntite[i]->pseudo, entites->listEntite[i]->score, entites->listEntite[i]->evolutionScore);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10 + 10 * i, message);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    }
    // On met à jour la fenêtre
    SDL_RenderPresent(renderer);
}
void afficherFin(int longueur, entites * entites){
    UNUSED(longueur);

    int nombreJoueurEnVie = 0;
    for (int i = 0; i < entites->nombreEntite; i++){
        if (entites->listEntite[i]->enVie){
            nombreJoueurEnVie++;
        }
    }

    if (nombreJoueurEnVie == 0 && entites->nombreEntite >= 2){
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10, "Partie nulle");
    } else if (entites->nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = entites->listEntite[0]->score;
        for (int i = 1; i < entites->nombreEntite; i++){
            if (entites->listEntite[i]->score > scoreMax){
                scoreMax = entites->listEntite[i]->score;
                vainqueur = i;
            }
        }
        char message[TAILLE_MAX_PSEUDO + 10];
        sprintf(message, " %s gagne ", entites->listEntite[vainqueur]->pseudo);
        SDL_SetRenderDrawColor(renderer, couleurs[vainqueur].r, couleurs[vainqueur].g, couleurs[vainqueur].b, couleurs[vainqueur].a);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10, message);
    } else if (nombreJoueurEnVie == 0){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10, "Defaite");
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10, "Victoire");
    }

    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    for (int i = 0; i < entites->nombreEntite; i++){
        char message[100];
        sprintf(message, "Joueur : %s\n", entites->listEntite[i]->pseudo);
        SDL_RenderDebugText(renderer, 10, 30 + 100 * i, message);
        sprintf(message, "Score : %d\n", entites->listEntite[i]->score);
        SDL_RenderDebugText(renderer, 10, 40 + 100 * i, message);
        sprintf(message, "Taille serpent : %d\n", entites->listEntite[i]->longueur);
        SDL_RenderDebugText(renderer, 10, 50 + 100 * i, message);
        sprintf(message, "Nombre de fruit mange : %d\n", entites->listEntite[i]->nombreFruitMange);
        SDL_RenderDebugText(renderer, 10, 60 + 100 * i, message);
        sprintf(message, "Nombre de fruit special mange : %d\n", entites->listEntite[i]->nombreFruitSpecialMange);
        SDL_RenderDebugText(renderer, 10, 70 + 100 * i, message);
        sprintf(message, "Distance parcouru : %d\n\n", entites->listEntite[i]->distanceParcourue);
        SDL_RenderDebugText(renderer, 10, 80 + 100 * i, message);
    }
    SDL_RenderPresent(renderer);
}

void detruireWindow(){
    if (renderer != NULL){
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}