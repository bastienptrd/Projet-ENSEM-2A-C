#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#include "gestionFichier.h"

//Lit un terrain depuis un fichier .txt
int** lireTerrain(char *fichier, int *largeur, int* longueur){
	char str[10000];
	char ch;
    *largeur = 0;
	FILE *f = fopen(fichier, "r");
    if (f == NULL){
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
	while (fgets(str,sizeof(str),f) != NULL){
		str[strcspn(str, "\r\n")] = '\0';
		*longueur = strlen(str);
		(*largeur)++;
	}
	fclose(f);

	int ** plateau = (int **)malloc(*largeur *sizeof(int *));
	for (int i = 0; i < *largeur;i++){
		plateau[i] = (int *)malloc(*longueur * sizeof(int));
	}	

	f = fopen(fichier,"r");
	int i = 0; int j = 0;
	while ((ch = getc(f)) != EOF) {
        if (ch != '\n') {
			if (j < *longueur){
				if (ch == CHAR_MURS) {
					plateau[i][j] = MUR;
				}else{
					plateau[i][j] = VIDE;
				}
				j++;
			}
        } else {
			j = 0;
			i++;
		}
	}
    return plateau;
}

//Ecrit le blian de la partie dans un .txt
void ecrire(char * fichier, char * pseudo, int score, int graine){
	FILE* f = fopen(fichier,"a");
    if (f == NULL){
        printf("Je ne peux pas ouvrir le fichier %s\n",fichier);
        exit(-1);
    }
    char buffer[80];

    time_t moment = time(NULL);
    struct tm * info = localtime(&moment);
    strftime(buffer, sizeof(buffer), "%d/%m/%Y à %H:%M:%S", info);

	fprintf(f, "%s à fait %d le %s sur la graine %d\n", pseudo, score, buffer, graine);
    fclose(f);
}