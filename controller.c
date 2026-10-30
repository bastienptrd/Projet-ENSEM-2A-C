#include "controller.h"

//Fait par IA
static struct termios ancienTerminal;

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

void *lireClavier(void *arg)
{
    entites * ents = arg;
    entite * ent = ents->listEntite[0];
    char c;

    while (1) {
        c = getchar();

        if (c != EOF) {
            pthread_mutex_lock(&dmutexjoueur1);
            if (c == 'z' && ent->direction != BAS){
                ent->direction = HAUT;
            }
            else if (c == 's' && ent->direction != HAUT){
                ent->direction = BAS;
            }
            else if (c == 'd' && ent->direction != GAUCHE){
                ent->direction = DROITE;
            }
            else if (c == 'q' && ent->direction != DROITE){
                ent->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur1);

            if (ents->nombreEntite == 1){
                continue;
            }


            if (c == 27){
                getchar();
                c = getchar();
            }
            pthread_mutex_lock(&dmutexjoueur2);

            if (c == 'A' && ents->listEntite[1]->direction != BAS){
                ents->listEntite[1]->direction = HAUT;
            }
            else if (c == 'B' && ents->listEntite[1]->direction != HAUT){
                ents->listEntite[1]->direction = BAS;
            }
            else if (c == 'C' && ents->listEntite[1]->direction != GAUCHE){
                ents->listEntite[1]->direction = DROITE;
            }
            else if (c == 'D' && ents->listEntite[1]->direction != DROITE){
                ents->listEntite[1]->direction = GAUCHE;
            }
            pthread_mutex_unlock(&dmutexjoueur2);

        }

        usleep(1000000 / FREQCLAVIER);
    }
    return NULL;
}