#include "../include/view.h"
#include "../include/utilitaire.h"
#include <stdio.h>
#include <SDL3/SDL.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;


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

SDL_Log("Fenetre et renderer initialises !");
}

void affichageTerrain(terrain * terrain, entite* listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    // On rempli l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On choisi la couleur bleu
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    int windowSizeX;
    int windowSizeY;
    SDL_GetWindowSize(window, &windowSizeX, &windowSizeY);

    int tailleCase = min(windowSizeX / terrain->longueur, windowSizeY / terrain->largeur);
    int decalageWindowX = windowSizeX / 2 - tailleCase * terrain->longueur / 2;
    int decalageWindowY = windowSizeY / 2 - tailleCase * terrain->largeur / 2;

    SDL_FRect caseRect = {0, 0, tailleCase, tailleCase};

    //Affichage du terrain et des entites dans le terminal
    for (int i = 0; i < terrain->largeur; i++){
        for (int j = 0; j < terrain->longueur; j++){
            
            caseRect.x = tailleCase * j + decalageWindowX;
            caseRect.y = tailleCase * i + decalageWindowY;

            //CaseVide permet de suivre si une entité a été affichée
            bool caseVide = true;

            //On parcourt l'ensemble des entités
            for (int k = 0; k < nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (listEntite[k]->posX == i && listEntite[k]->posY == j){
                    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
                    caseVide = false;
                }
            }

            //Si la case a déjà été dessiner on passe à la suivante
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
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            }

            SDL_RenderFillRect(renderer, &caseRect);
        }
    }
    
    // On met à jour la fenêtre
    SDL_RenderPresent(renderer);
}

void afficherTitre(int longueur){
    // On rempli l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On choisi la couleur bleu
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    int p;
    SDL_GetWindowSize(window, &p, NULL);

    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = 100;
    rect.w = (float)p;

    SDL_RenderFillRect(renderer, &rect);

    // On met à jour la fenêtre
    SDL_RenderPresent(renderer);
}
void afficherDecompte(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    affichageTerrain(terrain, listEntite, nombreEntite);
    sleep(2);
}
void afficherPartie(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    affichageTerrain(terrain, listEntite, nombreEntite);
}
void afficherFin(int longueur, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    printf("Pas fait fin\n");
    sleep(3);
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