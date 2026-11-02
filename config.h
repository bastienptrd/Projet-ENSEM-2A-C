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

#define DIRECTIONINITIALESERPENT -1
#define DIRECTIONDEFAULTSERPENT HAUT

//Taille
#define TAILLEINITIALESERPENT 3
//Taille ajouter au serpent par un fruit/fruit spécial
#define TAILLEAJOUTEEFRUIT 1
#define TAILLEAJOUTEEFRUITSPECIAL 1

//Séparateur corps des serpents des cases du terrain
#define CLASSESERPENT 1000

//Nombre d'essai de placement des serpents avant crash
#define NOMBREEESSAIMAX 10000

#define TAILLEMAXPSEUDO 20

#pragma endregion

#pragma region AFFICHAGE

//Char de l'affichage du terrain
#define CHARVIDE ' '
#define CHARMURS '#'
#define CHAROBSTACLES 'X'
#define CHARFRUITS '*'
#define CHARFRUITSPECIAL '@'
#define CHARSERPENT 'o'

//Nombre de retour à la ligne pour "effacer" l'affichage précédent
#define NOMBRERETOURCLEAN 15

#define NOMBREDECOMPTE 3
#define TEMPSCHIFFREDECOMPTE 2

//Nombre image par seconde de l'affichage et du déplacement
#define FREQDEPLACEMENT 2 

#pragma endregion

#pragma region FICHIER

//Noms des fichiers
#define CHEMINTERRAIN "plateau.txt"
#define CHEMINSCORE "scores.txt"

#pragma endregion

#pragma region CLAVIER

#define TOUCHEHAUT 'z'
#define TOUCHEBAS 's'
#define TOUCHEGAUCHE 'q'
#define TOUCHEDROITE 'd'


#pragma endregion

#endif