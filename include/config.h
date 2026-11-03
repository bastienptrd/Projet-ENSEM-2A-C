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
#define LARGEUR_TERRAIN 20
#define LONGEUR_TERRAIN 20

//Case
#define MUR 0 
#define OBSTACLE -1 //Bloc avec collision >=-1
#define VIDE -2
#define FRUIT -3
#define FRUIT_SPECIAL -4

#define FRUIT_INFINIE 0
#define NOMBRE_FRUIT_AJOUTER 1

#define SCORE_FRUIT 10
#define SCORE_FRUIT_SPECIAL 50

#pragma endregion

// ===================== ENTITE =====================
#pragma region ENTITE

enum direction {HAUT,BAS,GAUCHE,DROITE};

#define NOMBRE_MUR 10
#define NOMBRE_FRUIT 20
#define NOMBRE_MAX_ENTITE 2 // 2 serpents

#define FREQ_APPARITION_FRUIT_SPECIAL 50
#define TEMP_APPARITION_FRUIT_SPECIAL 10

#define DIRECTION_INITIALE_SERPENT -1
#define DIRECTION_DEFAULT_SERPENT HAUT

//Taille
#define TAILLE_INITIALE_SERPENT 3
//Taille ajouter au serpent par un fruit/fruit spécial
#define TAILLE_AJOUTEE_FRUIT 1
#define TAILLE_AJOUTEE_FRUIT_SPECIAL 1

//Séparateur corps des serpents des cases du terrain
#define CLASSE_SERPENT 1000

//Nombre d'essai de placement des serpents avant crash
#define NOMBRE_ESSAI_MAX 10000

#define TAILLE_MAX_PSEUDO 20

#pragma endregion

#pragma region AFFICHAGE

//Char de l'affichage du terrain
#define CHAR_VIDE ' '
#define CHAR_MURS '#'
#define CHAR_OBSTACLES 'X'
#define CHAR_FRUITS '*'
#define CHAR_FRUIT_SPECIAL '@'
#define CHAR_SERPENT 'o'

//Nombre de retour à la ligne pour "effacer" l'affichage précédent
#define NOMBRE_RETOUR_CLEAN 15

#define NOMBRE_DECOMPTE 3
#define TEMPS_CHIFFRES_DECOMPTE 2

//Nombre image par seconde de l'affichage et du déplacement
#define FREQ_DEPLACEMENT 2 

#pragma endregion

#pragma region FICHIER


//Noms des fichiers
#define CHEMIN_TERRAIN "assets/terrain/plateauVide.txt"
#define CHEMIN_SCORE "scores.txt"

#pragma endregion

#pragma region CLAVIER

#define TOUCHE_HAUT 'z'
#define TOUCHE_BAS 's'
#define TOUCHE_GAUCHE 'q'
#define TOUCHE_DROITE 'd'

#define FREQ_CLAVIER 10
#pragma endregion

#endif