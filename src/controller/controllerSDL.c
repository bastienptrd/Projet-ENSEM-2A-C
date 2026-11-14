#include <stdio.h>

#include "../../include/utilitaire.h"
#include "../../include/controller.h"
#include "../../include/configSDL.h"

static SDL_Event event;

void initialiserController(donneePartie * partie){
    UNUSED(partie);
}

void gererEventMenu(donneePartie * partie, int * fenetre) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            partie->launched = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_SPACE) {
                partie->nombreEntite = 2;
                *fenetre = PSEUDO;
            } else if (event.key.key == SDLK_S) {
                partie->nombreEntite = 1;
                partie->fruitInfinie = true;
                *fenetre = PSEUDO;
            }
        }
    }
}

void gererEventPseudo(donneePartie * partie, int * fenetre) {
    SDL_StartTextInput(window);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            partie->launched = false;
        } else if (event.type == SDL_EVENT_TEXT_INPUT && partie->indexLettrePseudo < 20){
            partie->listEntite[partie->indexEntitePseudo]->pseudo[partie->indexLettrePseudo] = event.text.text[0];
            partie->indexLettrePseudo++;
        } else if (event.type == SDL_EVENT_KEY_DOWN){
            if (event.key.key == SDLK_RETURN){
                if (partie->indexEntitePseudo + 1 >= partie->nombreEntite) {
                    *fenetre = DECOMPTE;
                } else {
                    partie->indexEntitePseudo++;
                    partie->indexLettrePseudo = strlen(partie->listEntite[partie->indexEntitePseudo]->pseudo);
                }
            } else if (event.key.key == SDLK_BACKSPACE) {
                partie->indexLettrePseudo = max(0, partie->indexLettrePseudo - 1);
                partie->listEntite[partie->indexEntitePseudo]->pseudo[partie->indexLettrePseudo] = '\0';
            } else if (event.key.key == SDLK_UP) {
                partie->indexEntitePseudo = max(0, partie->indexEntitePseudo - 1);
                partie->indexLettrePseudo = strlen(partie->listEntite[partie->indexEntitePseudo]->pseudo);
            } else if (event.key.key == SDLK_DOWN) {
                partie->indexEntitePseudo = min(partie->nombreEntite - 1, partie->indexEntitePseudo + 1);
                partie->indexLettrePseudo = strlen(partie->listEntite[partie->indexEntitePseudo]->pseudo);
            } else if (event.key.key == SDLK_ESCAPE) {
                *fenetre = MENU;
            }
        }
    }
    SDL_StopTextInput(window);
}
void gererEventDecompte(donneePartie * partie, int * fenetre){
    UNUSED(fenetre);

    entite * joueur1 = partie->listEntite[0];
    entite * joueur2 = partie->listEntite[1];
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            partie->launched = false;
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

            if (partie->nombreEntite == 1){
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
    if (difftime(time(NULL), partie->tempsDebut) >= NOMBRE_DECOMPTE * TEMPS_CHIFFRES_DECOMPTE) {
        *fenetre = JEU;
    }
}

void gererEventJeu(donneePartie * partie, int * fenetre){
    UNUSED(fenetre);

    entite * joueur1 = partie->listEntite[0];
    entite * joueur2 = partie->listEntite[1];

    while (SDL_PollEvent(&event)){
        if (event.type == SDL_EVENT_QUIT){
            partie->launched = false;
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

            if (partie->nombreEntite == 1){
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

void gererEventFin(donneePartie * partie, int * fenetre) {
    UNUSED(fenetre);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT){
            partie->launched = false;
        } else if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_RETURN) {
                *fenetre = MENU;
            } else if (event.key.key == SDLK_ESCAPE){
                *fenetre = MENU;
            }
        }
    }
}

void fermerController(){

}