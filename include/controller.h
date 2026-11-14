#pragma once

#include "partie.h"

void initialiserController(donneePartie * partie);
void gererEventMenu(donneePartie * partie, int * fenetre);
void gererEventPseudo(donneePartie * partie, int * fenetre);
void gererEventDecompte(donneePartie * partie, int * fenetre);
void gererEventJeu(donneePartie * partie, int * fenetre);
void gererEventFin(donneePartie * partie, int * fenetre);
void fermerController();