#include "../include/gestionFenetre.h"

#include <string.h>

#include "../include/controller.h"
#include "../include/view.h"

static void entreeFenetre(donneePartie * partie, int * fenetreOuverte) {

    if (*fenetreOuverte == START) {

        initWindow();

    } else if (*fenetreOuverte == MENU) {

    } else if (*fenetreOuverte == PSEUDO) {
        initialiserPartieEntite(partie);

        partie->indexEntitePseudo = 0;
        partie->indexLettrePseudo = strlen(partie->listEntite[partie->indexEntitePseudo]->pseudo);

    } else if (*fenetreOuverte == DECOMPTE) {

        initialiserPartieTerrain(partie);

        time(&partie->tempsDebut);

        //Selectionne une direction
        for (int i = 0; i < partie->nombreEntite; i++){
            partie->listEntite[i]->direction = DIRECTION_DEFAULT_SERPENT;
        }
        //Autorise les joueurs à changer librement de direction avant le début
        partie->pass = true;

        initialiserController(partie);


    } else if (*fenetreOuverte == JEU) {
        //Retire authorisation du changement libre
        partie->pass = false;
    } else if (*fenetreOuverte == FIN) {
        enregistrerPartie(partie);
    }
}

void gererEventFenetre(donneePartie * partie, int * fenetre) {
    int fenetreEntre = *fenetre;

    if (*fenetre == PASFENETRE) {
        *fenetre = START;
    } else if (*fenetre == START) {
        // Chargement, mais rien à charger pour l'instant
        *fenetre = MENU;
    } else if (*fenetre == MENU) {
        gererEventMenu(partie, fenetre);
    } else if (*fenetre == PSEUDO) {
        gererEventPseudo(partie, fenetre);
    } else if (*fenetre == DECOMPTE) {
        gererEventDecompte(partie, fenetre);
    } else if (*fenetre == JEU) {
        gererEventJeu(partie, fenetre);
        testFin(partie, fenetre);
    } else if (*fenetre == FIN) {
        gererEventFin(partie, fenetre);
    }

    if (*fenetre != fenetreEntre) {
        entreeFenetre(partie, fenetre);
    }
}

void afficherFenetre(donneePartie * partie, int * fenetre) {
    if (*fenetre == MENU) {
        afficherTitre(LONGEUR_TERRAIN);
    } else if (*fenetre == PSEUDO) {
        afficherPseudo(partie);
    } else if (*fenetre == DECOMPTE) {
        afficherDecompte(partie);
    } else if (*fenetre == JEU) {
        afficherPartie(partie);
    } else if (*fenetre == FIN) {
        afficherFin(partie, partie->terrain->longueur);
    }
}
