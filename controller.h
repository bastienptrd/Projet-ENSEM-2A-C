#include <termios.h>

#include "entite.h"

#define FREQCLAVIER 10

static struct termios origtc, newtc;

void * lire_clavier(void *arg);