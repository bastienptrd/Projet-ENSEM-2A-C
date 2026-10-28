#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ===================== TERRAIN =====================
#pragma region TERRAIN

//Taille
#define LARGEURTERRAIN 10
#define LONGEURTERRAIN 20

//Case
#define VIDE 0
#define MUR -1
#define FRUIT -2
#define FRUITSPECIAL -3

#pragma endregion

// ===================== ENTITE =====================
#pragma region ENTITE

#define NOMBREFRUIT 5
#define NOMBREMAXENTITE 8 // 2 serpents + 5 fruits + fruit spéciale
#define TAILLEINITIALESERPENT 3

#pragma endregion