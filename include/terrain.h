#pragma once

typedef struct {
    int ** tableau;
    int largeur;
    int longueur;
} terrain;

terrain * creerTerrain(int largeur, int longeur);    //Créer le tableau du terrain
void initTerrain(terrain * terrain);    //Place des murs sur toutes les côtés
void ajouterClasseTerrain(terrain * terrain, int num, int val); //Ajoute val aux serpents dont la classe est num
int nombreCaseObj(terrain * terrain, int obj);  //Renvoie le nombre de case ayant obj pour valeur
void ajouterAleatoireTerrain(terrain * terrain, int nombre, int obj, int* posX, int* posY);
void freeTerrain(terrain * terrain);    //Libération en mémoire du terrain