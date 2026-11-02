int somme(int * list, int taille){
    int somme = 0;
    for (int i = 0; i < taille; i++){
        somme += list[i];
    }
    return somme;
}