#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/controller.h"

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
    entites * ents = arg;
    entite * joueur1 = ents->listEntite[0];
    entite * joueur2 = ents->listEntite[1];

    while (*(ents->launched)) {
        char c = getchar();

        if (c != EOF) {
            pthread_mutex_lock(&dmutexjoueur1);
            if (c == TOUCHE_HAUT && (joueur1->direction != BAS || ents->pass)){
                joueur1->direction = HAUT;
            }
            else if (c == TOUCHE_BAS && (joueur1->direction != HAUT || ents->pass)){
                joueur1->direction = BAS;
            }
            else if (c == TOUCHE_DROITE && (joueur1->direction != GAUCHE || ents->pass)){
                joueur1->direction = DROITE;
            }
            else if (c == TOUCHE_GAUCHE && (joueur1->direction != DROITE || ents->pass)){
                joueur1->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur1);

            if (ents->nombreEntite == 1){
                continue;
            }


            if (c == 27){
                getchar();
                c = getchar();
            } else {
                continue;
            }
            pthread_mutex_lock(&dmutexjoueur2);

            if (c == 'A' && (joueur2->direction != BAS || ents->pass)){
                joueur2->direction = HAUT;
            }
            else if (c == 'B' && (joueur2->direction != HAUT || ents->pass)){
                joueur2->direction = BAS;
            }
            else if (c == 'C' && (joueur2->direction != GAUCHE || ents->pass)){
                joueur2->direction = DROITE;
            }
            else if (c == 'D' && (joueur2->direction != DROITE || ents->pass)){
                joueur2->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur2);

        }

        usleep(1000000 / FREQ_CLAVIER);
    }

    return NULL;
}


void initialiserController(entites * entites){
    initialiserTerminal();

    //Recuperation des entrées clavier
    pthread_create(&clavier, NULL, lireClavier, entites);
}


void lirePseudo(char pseudo[TAILLE_MAX_PSEUDO]){
    printf("Entrez votre pseudo : ");
    scanf("%s", pseudo);
}

void gererEvent(entites * entites){
    UNUSED(entites);
}

void fermerController(){
    
    //On arrete le thread clavier
    pthread_cancel(clavier);
    pthread_join(clavier, NULL);

    restaurerTerminal();
}