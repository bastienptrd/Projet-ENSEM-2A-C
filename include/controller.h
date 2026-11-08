#include <termios.h>

#include "entite.h"

typedef struct{
    entite * listEntite[NOMBRE_MAX_ENTITE];
    int nombreEntite;
    bool pass;
    bool * launched;
} entites;

//Fait par IA
void initialiserTerminal(void);
void restaurerTerminal(void);

void * lireClavier(void *arg);

void initialiserController(entites * entites);
void gererEvent(entites * entites);
void fermerController();