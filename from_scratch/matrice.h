#pragma once
typedef struct {
    int n;
    int m;
    float** mat;
} matrice;

//Créer une matrice nulle de taille n*m
matrice matrice_nulle(int n, int m);

//Créer un pointeur vers une matrice nulle de taille n*m
matrice* matrice_nulle_pointeur(int n, int m);


//Affiche la matrice a
void print_matrice (matrice a);

//Libère la mémoire allouer pour la matrice a
void free_matrice(matrice a);

//Renvoie une nouvelle matrice, somme des matrices a et b
matrice somme(matrice a, matrice b);

//Renvoie une nouvelle matrice, produit des matrices a et b
matrice produit(matrice a, matrice b);

void multiplication_scalaire(matrice a, float lambda);

//Renvoie une nouvelle matrice, transposée de la matrice a
matrice transposee(matrice a);

// Fonction pour concaténer deux matrices horizontalement
matrice concatenation2(matrice a, matrice b) ;

// Fonction pour concaténer trois matrices horizontalement
matrice concatenation3(matrice a, matrice b, matrice c) ;


void echange_ligne(matrice* a, int i, int j);
void multiplication_ligne(matrice* a, int i, float lambda);
void ajout_ligne(matrice* a, int i, int j, float lambda);
int choix_pivot_naïf(matrice *a, int i);
int choix_pivot_partiel(matrice *a, int i);
//Effectue un pivot de Gauss sur la matrice* a
int Gauss_Jordan(matrice* a);
//Effectue un pivot de Gauss sur la matrice* a
int Gauss_Jordan_print(matrice* a);
matrice* inverser_matrice(matrice* a);
matrice* inverser_matrice_print(matrice* a);

matrice* resolution_systeme(matrice* a,matrice* v);
matrice* resolution_systeme_print(matrice* a,matrice* v);
