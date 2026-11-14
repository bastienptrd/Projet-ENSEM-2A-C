#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "../../include/controller.h"

//Detection clavier
static pthread_t clavier;

//Fait par IA
static struct termios ancienTerminal;
//Fait par IA
static void initialiserTerminal(void)
{
    struct termios nouveauTerminal;

    tcgetattr(STDIN_FILENO, &ancienTerminal);

    nouveauTerminal = ancienTerminal;
    nouveauTerminal.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &nouveauTerminal);
}
//Fait par IA
static void restaurerTerminal(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &ancienTerminal);
}

//Récupère les entrées clavier
static void *lireClavier(void *arg)
{
    donneePartie * partie = arg;
    entite * joueur1 = partie->listEntite[0];
    entite * joueur2 = partie->listEntite[1];

    while (partie->launched) {
        char c = getchar();

        if (c != EOF) {
            pthread_mutex_lock(&dmutexjoueur1);
            if (c == TOUCHE_HAUT && (joueur1->direction != BAS || partie->pass)){
                joueur1->direction = HAUT;
            }
            else if (c == TOUCHE_BAS && (joueur1->direction != HAUT || partie->pass)){
                joueur1->direction = BAS;
            }
            else if (c == TOUCHE_DROITE && (joueur1->direction != GAUCHE || partie->pass)){
                joueur1->direction = DROITE;
            }
            else if (c == TOUCHE_GAUCHE && (joueur1->direction != DROITE || partie->pass)){
                joueur1->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur1);

            if (partie->nombreEntite == 1){
                continue;
            }


            if (c == 27){
                getchar();
                c = getchar();
            } else {
                continue;
            }
            pthread_mutex_lock(&dmutexjoueur2);

            if (c == 'A' && (joueur2->direction != BAS || partie->pass)){
                joueur2->direction = HAUT;
            }
            else if (c == 'B' && (joueur2->direction != HAUT || partie->pass)){
                joueur2->direction = BAS;
            }
            else if (c == 'C' && (joueur2->direction != GAUCHE || partie->pass)){
                joueur2->direction = DROITE;
            }
            else if (c == 'D' && (joueur2->direction != DROITE || partie->pass)){
                joueur2->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur2);

        }

        usleep(1000000 / FREQ_CLAVIER);
    }

    return NULL;
}


void initialiserController(donneePartie * partie){
    initialiserTerminal();

    //Recuperation des entrées clavier
    pthread_create(&clavier, NULL, lireClavier, partie);
}

void gererEventMenu(donneePartie * partie, int * fenetre){
    UNUSED(partie);
    *fenetre = PSEUDO;
}

void gererEventPseudo(donneePartie * partie, int * fenetre) {

    printf("Entrez votre pseudo joueur %d : ", partie->indexEntitePseudo + 1);
    scanf("%s", partie->listEntite[partie->indexEntitePseudo]->pseudo);
    partie->indexEntitePseudo++;
    if (partie->indexEntitePseudo == partie->nombreEntite) {
        partie->indexEntitePseudo = 0;
        *fenetre = DECOMPTE;
    }
}

void gererEventDecompte(donneePartie * partie, int * fenetre){
    UNUSED(partie);
    *fenetre = JEU;
}
void gererEventJeu(donneePartie * partie, int * fenetre){
    UNUSED(partie);
    UNUSED(fenetre);
}
void gererEventFin(donneePartie * partie, int * fenetre){
    UNUSED(fenetre);

    pthread_cancel(clavier);
    pthread_join(clavier, NULL);

    restaurerTerminal();

    char reponse;
    printf("Rejouer ? (y,n)");
    scanf("%c", &reponse);
    if (reponse == 'y') {
        *fenetre = MENU;
    } else {
        *fenetre = PASFENETRE;
        partie->launched = false;
    }
}

void fermerController(){

}