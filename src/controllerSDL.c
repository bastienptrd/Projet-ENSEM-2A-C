#include <stdio.h>

#include "../include/utilitaire.h"
#include "../include/controller.h"
#include "../include/configSDL.h"

static SDL_Event event;

void initialiserController(entites * entites){
    UNUSED(entites);
}

void gererEventMenu(entites * entites, int * fenetre) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_SPACE) {
                *fenetre = PSEUDO;
            } else if (event.key.key == SDLK_S) {
                entites->nombreEntite = 1;
                *fenetre = PSEUDO;
            }
        }
    }
}

void gererEventPseudo(entites * entites, int * fenetre, int * indexLettrePseudo, int * indexEntitePseudo) {
    SDL_StartTextInput(window);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_TEXT_INPUT && *indexLettrePseudo < 20){
            entites->listEntite[*indexEntitePseudo]->pseudo[*indexLettrePseudo] = event.text.text[0];
            (*indexLettrePseudo)++;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == SDLK_RETURN){
                (*indexEntitePseudo)++;
                *indexLettrePseudo = 0;
                if (*indexEntitePseudo >= entites->nombreEntite) {
                    *fenetre = DECOMPTE;
                }
            } else if (event.key.key == SDLK_BACKSPACE) {
                *indexLettrePseudo = max(0, *indexLettrePseudo - 1);
                entites->listEntite[*indexEntitePseudo]->pseudo[*indexLettrePseudo] = '\0';
            } else if (event.key.key == SDLK_UP) {
                *indexEntitePseudo = max(0, *indexEntitePseudo - 1);
                *indexLettrePseudo = strlen(entites->listEntite[*indexEntitePseudo]->pseudo);
            } else if (event.key.key == SDLK_DOWN) {
                *indexEntitePseudo = min(entites->nombreEntite - 1, *indexEntitePseudo + 1);
                *indexLettrePseudo = strlen(entites->listEntite[*indexEntitePseudo]->pseudo);
            }
        }
    }
    SDL_StopTextInput(window);
}
void gererEventDecompte(entites * entites, int * fenetre, time_t tempsDebut){
    UNUSED(fenetre);

    entite * joueur1 = entites->listEntite[0];
    entite * joueur2 = entites->listEntite[1];
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == (SDL_Keycode)TOUCHE_HAUT){
                joueur1->direction = HAUT;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_BAS){
                joueur1->direction = BAS;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_DROITE){
                joueur1->direction = DROITE;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_GAUCHE){
                joueur1->direction = GAUCHE;
            }

            if (entites->nombreEntite == 1){
                continue;
            }

            if (event.key.key == SDLK_UP ){
                joueur2->direction = HAUT;
            }
            else if (event.key.key == SDLK_DOWN){
                joueur2->direction = BAS;
            }
            else if (event.key.key == SDLK_RIGHT){
                joueur2->direction = DROITE;
            }
            else if (event.key.key == SDLK_LEFT){
                joueur2->direction = GAUCHE;
            }
        }
    }
    if (difftime(time(NULL), tempsDebut) >= NOMBRE_DECOMPTE * TEMPS_CHIFFRES_DECOMPTE) {
        *fenetre = JEU;
    }
}

void gererEventJeu(entites * entites, int * fenetre){
    UNUSED(fenetre);

    entite * joueur1 = entites->listEntite[0];
    entite * joueur2 = entites->listEntite[1];

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            
            pthread_mutex_lock(&dmutexjoueur1);
            if (event.key.key == (SDL_Keycode)TOUCHE_HAUT && joueur1->direction != BAS){
                joueur1->direction = HAUT;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_BAS && joueur1->direction != HAUT){
                joueur1->direction = BAS;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_DROITE && joueur1->direction != GAUCHE){
                joueur1->direction = DROITE;
            }
            else if (event.key.key == (SDL_Keycode)TOUCHE_GAUCHE && joueur1->direction != DROITE){
                joueur1->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur1);

            if (entites->nombreEntite == 1){
                continue;
            }

            pthread_mutex_lock(&dmutexjoueur2);
            if (event.key.key == SDLK_UP && joueur2->direction != BAS){
                joueur2->direction = HAUT;
            }
            else if (event.key.key == SDLK_DOWN && joueur2->direction != HAUT){
                joueur2->direction = BAS;
            }
            else if (event.key.key == SDLK_RIGHT && joueur2->direction != GAUCHE){
                joueur2->direction = DROITE;
            }
            else if (event.key.key == SDLK_LEFT && joueur2->direction != DROITE){
                joueur2->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur2);
        }
    }
}

void gererEventFin(entites * entites, int * fenetre) {
    UNUSED(fenetre);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        }
    }
}

void fermerController(){

}