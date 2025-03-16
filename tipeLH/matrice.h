#ifndef matrice_H
#define Matrice_H

#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int n;
    int m;
    long double** mat;
} matrice;

//Créer une matrice nulle de taille n*m
matrice matrice_nulle(int n, int m);

//Créer un pointeur vers une matrice nulle de taille n*m
matrice* matrice_nulle_pointeur(int n, int m);


// Crée une matrice identité de taille n*n
matrice matrice_identite(int n);

// Calcule la norme d'un vecteur (colonne de la matrice)
long double norme_vecteur(matrice a, int colonne);

//Affiche la matrice a
void print_matrice (matrice a);

//Teste l'égalité entre les matrice a et b, renvoie true si les matrices sont egales, false sinon
bool matrice_egale (matrice a, matrice b);

//Libère la mémoire allouer pour la matrice a
void free_matrice(matrice a);

//Renvoie une nouvelle matrice, somme des matrices a et b
matrice somme(matrice a, matrice b);

//Renvoie une nouvelle matrice, produit des matrices a et b
matrice produit(matrice a, matrice b);

void multiplication_scalaire(matrice a, long double lambda);

//Renvoie une nouvelle matrice, transposée de la matrice a
matrice transposee(matrice a);

// Fonction pour concaténer deux matrices horizontalement
matrice concatenation2(matrice a, matrice b) ;

// Fonction pour concaténer trois matrices horizontalement
matrice concatenation3(matrice a, matrice b, matrice c) ;

matrice copie(matrice a);
void echange_ligne(matrice* a, int i, int j);
void multiplication_ligne(matrice* a, int i, long double lambda);
void ajout_ligne(matrice* a, int i, int j, long double lambda);
void set_colonne(matrice *target, int col_idx, matrice column_vector);
// Fonction pour extraire la i-ème colonne de la matrice A
matrice matrice_colonne(matrice A, int i);
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

/*void perturber_matrice(matrice* A, long double epsilon) ;
matrice* resolution_systeme_non_nulle(matrice* A, matrice* V);
*/

matrice pseudo_inverser_matrice(matrice* a);
#endif
