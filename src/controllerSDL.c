#include <stdio.h>

#include "../include/utilitaire.h"
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

void gererEventMenu(entites * entites, int * fenetre) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_SPACE) {
                *fenetre = PSEUDO;
            }
        }
    }
}

void gererEventPseudo(entites * entites, int * fenetre, int * i, int * numEntite) {
    SDL_StartTextInput(window);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            *(entites->launched) = false;
        } else if (event.type == SDL_EVENT_TEXT_INPUT && *i < 20){
            entites->listEntite[*numEntite]->pseudo[*i] = event.text.text[0];
            (*i)++;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == SDLK_RETURN){
                (*numEntite)++;
                *i = 0;
                if (*numEntite >= entites->nombreEntite) {
                    *fenetre = DECOMPTE;
                }
            } else if (event.key.key == SDLK_BACKSPACE) {
                *i = max(0, *i - 1);
                entites->listEntite[*numEntite]->pseudo[*i] = '\0';
            } else if (event.key.key == SDLK_UP) {
                *numEntite = max(0, *numEntite - 1);
                *i = strlen(entites->listEntite[*numEntite]->pseudo);
            } else if (event.key.key == SDLK_DOWN) {
                *numEntite = min(entites->nombreEntite - 1, *numEntite + 1);
                *i = strlen(entites->listEntite[*numEntite]->pseudo);
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