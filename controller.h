#include <termios.h>

#include "entite.h"

#define FREQCLAVIER 10

typedef struct{
    entite * listEntite[NOMBREMAXENTITE];
    int nombreEntite;
} entites;

//Fait par IA
void initialiserTerminal(void);
void restaurerTerminal(void);

void * lireClavier(void *arg);