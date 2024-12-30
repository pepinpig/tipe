#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"

// Calcule la norme d'un vecteur ligne
float norme_vecteur_ligne(matrice a, int ligne) {
    float somme = 0.0;
    for (int j = 0; j < a.m; j++) {
        somme += a.mat[ligne][j] * a.mat[ligne][j];
    }
    return sqrt(somme);
}

void decomposition_RQ(matrice* A, matrice* R, matrice* Q) {
    int n = A->n;
    int m = A->m;
    *R = matrice_nulle(n, m);
    *Q = matrice_identite(m);

    // Appliquer Gram-Schmidt en partant de la dernière ligne de A
    for (int i = n - 1; i >= 0; i--) {
        // Initialiser le vecteur de la ligne actuelle
        for (int j = 0; j < m; j++) {
            Q->mat[i][j] = A->mat[i][j];
        }
        
        // Orthogonalisation par rapport aux lignes précédentes
        for (int k = i + 1; k < n; k++) {
            float dot_product = 0.0;
            for (int j = 0; j < m; j++) {
                dot_product += Q->mat[k][j] * A->mat[i][j];
            }
            R->mat[i][k] = dot_product;
            for (int j = 0; j < m; j++) {
                Q->mat[i][j] -= R->mat[i][k] * Q->mat[k][j];
            }
        }

        // Normalisation de la ligne
        R->mat[i][i] = norme_vecteur_ligne(*Q, i);
        for (int j = 0; j < m; j++) {
            Q->mat[i][j] /= R->mat[i][i];
        }
    }
}

int main() {
    int n = 3; // Exemple pour une matrice 3x3
    matrice A = matrice_nulle(n, n);
    
    // Remplir A avec des valeurs pour l'exemple
    A.mat[0][0] = 1; A.mat[0][1] = 2; A.mat[0][2] = 3;
    A.mat[1][0] = 4; A.mat[1][1] = 5; A.mat[1][2] = 6;
    A.mat[2][0] = 7; A.mat[2][1] = 8; A.mat[2][2] = 9;

    matrice R, Q;
    decomposition_RQ(&A, &R, &Q);

    printf("Matrice R:\n");
    print_matrice(R);
    printf("\nMatrice Q:\n");
    print_matrice(Q);

    // Libérer la mémoire
    free_matrice(A);
    free_matrice(R);
    free_matrice(Q);
    return 0;
}