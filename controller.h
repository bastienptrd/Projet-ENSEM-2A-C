#include <termios.h>

#include "entite.h"

typedef struct{
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
} entites;

//Fait par IA
void initialiserTerminal(void);
void restaurerTerminal(void);

void * lireClavier(void *arg);