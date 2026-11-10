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

static void affichageTerrain(terrain * terrain, entite* listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
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
            for (int k = 0; k < nombreEntite; k++){

                //Si une entité est présente à ces coordonnées
                if (listEntite[k]->posX == i && listEntite[k]->posY == j){
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
    
    // On met à jour la fenêtre
    SDL_RenderPresent(renderer);
}

void afficherTitre(int longueur){
    UNUSED(longueur);

    // On remplit l'écran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On choisit la couleur bleue
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

    int windowSizeX = 0;
    int windowSizeY = 0;
    SDL_GetWindowSize(window, &windowSizeX, &windowSizeY);

    affichageTerrain(terrain, listEntite, nombreEntite);
    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.h = POSITION_P_PLATEAU_JEU_Y * windowSizeX;
    rect.w = POSITION_P_PLATEAU_JEU_Y * windowSizeY;

    for (int i = 0; i < NOMBRE_DECOMPTE; i++){
        rect.x = POSITION_P_PLATEAU_JEU_Y * windowSizeY * i;
            
        SDL_SetRenderDrawColor(renderer, 255 * (i <= 1) , 255 * (i >= 1), 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        sleep(TEMPS_CHIFFRES_DECOMPTE);
    }

}
void afficherPartie(terrain * terrain, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    affichageTerrain(terrain, listEntite, nombreEntite);
}
void afficherFin(int longueur, entite * listEntite[NOMBRE_MAX_ENTITE], int nombreEntite){
    UNUSED(longueur);

    int nombreJoueurEnVie = 0;
    for (int i = 0; i < nombreEntite; i++){
        if (listEntite[i]->enVie){
            nombreJoueurEnVie++;
        }
    }
    
    if (nombreJoueurEnVie == 0 && nombreEntite >= 2){
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);
    } else if (nombreEntite >= 2){
        int vainqueur = 0;
        int scoreMax = listEntite[0]->score;
        for (int i = 1; i < nombreEntite; i++){
            if (listEntite[i]->score > scoreMax){
                scoreMax = listEntite[i]->score;
                vainqueur = i;
            }
        }
        SDL_SetRenderDrawColor(renderer, couleurs[vainqueur].r, couleurs[vainqueur].g, couleurs[vainqueur].b, couleurs[vainqueur].a);
        SDL_RenderClear(renderer);
    } else if (nombreJoueurEnVie == 0){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
    }
    SDL_RenderPresent(renderer);
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