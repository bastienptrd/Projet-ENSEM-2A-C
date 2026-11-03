#include "controller.h"

//Fait par IA
static struct termios ancienTerminal;
//Fait par IA
void initialiserTerminal(void)
{
    struct termios nouveauTerminal;

    tcgetattr(STDIN_FILENO, &ancienTerminal);

    nouveauTerminal = ancienTerminal;
    nouveauTerminal.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &nouveauTerminal);
}
//Fait par IA
void restaurerTerminal(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &ancienTerminal);
}

//Récupère les entrées clavier
void *lireClavier(void *arg)
{
    entites * ents = arg;
    entite * joueur1 = ents->listEntite[0];
    entite * joueur2 = ents->listEntite[1];
    char c;

    while (1) {
        c = getchar();

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
}