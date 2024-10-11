#pragma once
typedef struct {
    int n;
    int m;
    int** mat;
} matrice;

//Créer une matrice nulle de taille n*m
matrice matrice_nulle(int n, int m);


//Affiche la matrice a
void print_matrice (matrice a);

//Libère la mémoire allouer pour la matrice a
void free_matrice(matrice a);

//Renvoie une nouvelle matrice, somme des matrices a et b
matrice somme(matrice a, matrice b);

//Renvoie une nouvelle matrice, produit des matrices a et b
matrice produit(matrice a, matrice b);

//Renvoie une nouvelle matrice, transposée de la matrice a
matrice transposee(matrice a);


