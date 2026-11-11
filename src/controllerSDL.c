#include <stdio.h>
#include "../include/controller.h"
#include "../include/configSDL.h"

static SDL_Event event;

void initialiserController(entites * entites){
    UNUSED(entites);
}

void lirePseudo(char pseudo[TAILLE_MAX_PSEUDO]){
    SDL_StartTextInput(window);
    int i = 0;
    bool launched = true;
    while (launched){
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // On choisit la couleur blanche
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_SetRenderScale(renderer, 3.0f, 3.0f);
        SDL_RenderDebugText(renderer, 10, 10, "SNAKE");
        SDL_RenderDebugText(renderer, 10, 30, "Entrez pseudo : ");

        SDL_RenderDebugText(renderer, 10, 50, pseudo);

        SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                launched = false;
            } else if (event.type == SDL_EVENT_TEXT_INPUT && i < 20){
                pseudo[i] = event.text.text[0];
                i++;
            } else if (event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_RETURN){
                    launched = false;
                } else if (event.key.key == SDLK_BACKSPACE) {
                    i--;
                    pseudo[i] = '\0';
                }
            }
        }
    }
    printf("%s\n", pseudo);
    fflush(stdout);
    SDL_StopTextInput(window);
}

void gererEvent(entites * entites){
    
    entite * joueur1 = entites->listEntite[0];
    entite * joueur2 = entites->listEntite[1];

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            
            pthread_mutex_lock(&dmutexjoueur1);
            if (event.key.key == (SDL_Keycode)TOUCHE_HAUT && (joueur1->direction != BAS || entites->pass)){
                joueur1->direction = HAUT;
            }
            else if (event.key.key == (SDL_Keycode)(TOUCHE_BAS) && (joueur1->direction != HAUT || entites->pass)){
                joueur1->direction = BAS;
            }
            else if (event.key.key == (SDL_Keycode)(TOUCHE_DROITE) && (joueur1->direction != GAUCHE || entites->pass)){
                joueur1->direction = DROITE;
            }
            else if (event.key.key == (SDL_Keycode)(TOUCHE_GAUCHE) && (joueur1->direction != DROITE || entites->pass)){
                joueur1->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur1);

            if (entites->nombreEntite == 1){
                continue;
            }

            pthread_mutex_lock(&dmutexjoueur2);
            if (event.key.key == SDLK_UP && (joueur2->direction != BAS || entites->pass)){
                joueur2->direction = HAUT;
            }
            else if (event.key.key == SDLK_DOWN && (joueur2->direction != HAUT || entites->pass)){
                joueur2->direction = BAS;
            }
            else if (event.key.key == SDLK_RIGHT && (joueur2->direction != GAUCHE || entites->pass)){
                joueur2->direction = DROITE;
            }
            else if (event.key.key == SDLK_LEFT && (joueur2->direction != DROITE || entites->pass)){
                joueur2->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur2);
        }
    }
}

void fermerController(){

}