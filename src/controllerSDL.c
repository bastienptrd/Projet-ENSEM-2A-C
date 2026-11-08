#include "controller.h"
#include <SDL3/SDL.h>

SDL_Event event;

void initialiserController(entites * entites){

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