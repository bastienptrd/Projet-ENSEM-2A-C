#pragma once

#include "entite.h"

void initialiserController(entites * entites);
void gererEventMenu(entites * entites, int * fenetre);
void gererEventPseudo(entites * entites, int * fenetre, int * i, int * indexEntitePseudo);
void gererEventDecompte(entites * entites, int * fenetre, time_t tempsDebut);
void gererEventJeu(entites * entites, int * fenetre);
void gererEventFin(entites * entites, int * fenetre);
void fermerController();