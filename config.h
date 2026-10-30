#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

extern pthread_mutex_t dmutexjoueur1;
extern pthread_mutex_t dmutexjoueur2;

// ===================== TERRAIN =====================
#pragma region TERRAIN

//Taille
#define LARGEURTERRAIN 20
#define LONGEURTERRAIN 20

//Case
#define MUR 0 
#define OBSTACLE -1 //Bloc avec collision >=-1
#define VIDE -2
#define FRUIT -3
#define FRUITSPECIAL -4

#define FRUITINFINIE 0
#define NOMBREFRUITAJOUTER 1

#define SCOREFRUIT 10
#define SCOREFRUITSPECIAL 50
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

//Taille
#define TAILLEINITIALESERPENT 3
#define TAILLEAJOUTEEFRUIT 1
#define TAILLEAJOUTEEFRUITSPECIAL 1

//Séparateur case terrain
#define CLASSESERPENT 1000

//Apparition serpent
#define NOMBREEESSAIMAX 10000

#pragma endregion

#pragma region AFFICHAGE

#define CHARVIDE ' '
#define CHARMURS '#'
#define CHAROBSTACLES 'X'
#define CHARFRUITS '*'
#define CHARFRUITSPECIAL '@'

#define NOMBRERETOURCLEAN 15

#define FREQAFFICHAGE 2 //image par sec
#pragma endregion

#endif