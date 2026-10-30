#include "gestionFichier.h"

int** lireTerrain(char *fichier, int *largeur, int* longueur){
	FILE *f;
	char str[10000];
	int i,j;
	char ch;
    *largeur = 0;
	f = fopen(fichier,"r");
    if (f == NULL){
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
	while (fgets(str,sizeof(str),f) != NULL){
		str[strlen(str)-1] = '\0';
		*longueur = strlen(str);
		(*largeur)++;
	}
	fclose(f);
	int ** plateau = (int **)malloc(*largeur *sizeof(int *));
	for (i=0; i < *largeur;i++){
		plateau[i] = (int *)malloc(*longueur * sizeof(int));
	}	
	f = fopen(fichier,"r");
	i = 0; j = 0;
	while ((ch = getc(f)) != EOF) {
        if (ch != '\n') {
            if (ch == CHARMURS) {
                plateau[i][j] = MUR;
            }else{
                plateau[i][j] = VIDE;
            }
            j++;
            if (j == *longueur) {
                j = 0;
                i++;
            }
        }
	}
    return plateau;
}