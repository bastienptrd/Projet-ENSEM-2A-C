#include "controller.h"

//Fait par IA
static void restaurer_terminal(void *arg)
{
    (void)arg;
    tcsetattr(STDIN_FILENO, TCSANOW, &origtc);
}

void *lire_clavier(void *arg)
{
    entite *ent = arg;
    char c;

    tcgetattr(STDIN_FILENO, &origtc);
    newtc = origtc;
    newtc.c_lflag &= ~(ICANON | ECHO);

    pthread_cleanup_push(restaurer_terminal, NULL);

    tcsetattr(STDIN_FILENO, TCSANOW, &newtc);

    while (1) {
        c = getchar();

        if (c != EOF) {
            pthread_mutex_lock(&dmutex);

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

            pthread_mutex_unlock(&dmutex);
        }

        usleep(1000000 / FREQCLAVIER);
    }

    pthread_cleanup_pop(1);
    return NULL;
}