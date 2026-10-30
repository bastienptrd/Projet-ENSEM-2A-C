#ifndef CONFIG_H
#define CONFIG_H

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
#define MUR 0 
#define OBSTACLE -1 //Bloc avec collision >=-1
#define VIDE -2
#define FRUIT -3
#define FRUITSPECIAL -4

#pragma endregion

// ===================== ENTITE =====================
#pragma region ENTITE

enum direction {HAUT,BAS,GAUCHE,DROITE};

#define NOMBREMUR 10
#define NOMBREFRUIT 5
#define NOMBREMAXENTITE 2 // 2 serpents

#define FREQAPPARITIONFRUITSPECIAL 50
#define TEMPAPPARITIONFRUITSPECIAL 10

#define DIRECTIONINITIALESERPENT HAUT
#define TAILLEINITIALESERPENT 3
#define CLASSESERPENT 1000

#define NOMBREEESAIMAX 10000

#pragma endregion

#pragma region AFFICHAGE

#define CHARMURS '#'
#define CHAROBSTACLES 'X'
#define CHARFRUITS '*'
#define CHARFRUITSPECIAL '@'

#define NOMBRERETOURCLEAN 15

#define FREQAFFICHAGE 2 //image par sec
#pragma endregion

#endif