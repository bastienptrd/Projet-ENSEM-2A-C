#pragma once

#include "entite.h"

void initialiserController(donneePartie * partie);
void gererEventMenu(donneePartie * partie, int * fenetre);
void gererEventPseudo(donneePartie * partie, int * fenetre, int * i, int * indexEntitePseudo);
void gererEventDecompte(donneePartie * partie, int * fenetre);
void gererEventJeu(donneePartie * partie, int * fenetre);
void gererEventFin(donneePartie * partie, int * fenetre);
void fermerController();