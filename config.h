#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

extern pthread_mutex_t dmutex;

// ===================== TERRAIN =====================
#pragma region TERRAIN

//Taille
#define LARGEURTERRAIN 10
#define LONGEURTERRAIN 20

//Case
#define MUR 0 //Bloc avec collision >=0
#define VIDE -1
#define FRUIT -2
#define FRUITSPECIAL -3

#pragma endregion

// ===================== ENTITE =====================
#pragma region ENTITE

enum direction {HAUT,BAS,GAUCHE,DROITE};

#define NOMBREMUR 10
#define NOMBREFRUIT 5
#define NOMBREMAXENTITE 8 // 2 serpents + 5 fruits + fruit spéciale

#define TAILLEINITIALESERPENT 3
#define CLASSESERPENT 1000

#pragma endregion