#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"

// Fonction pour calculer la norme d'un vecteur colonne
long double norme_vecteur_colonne(matrice a, int colonne) {
    long double somme = 0.0;
    for (int i = 0; i < a.n; i++) {
        somme += a.mat[i][colonne] * a.mat[i][colonne];
    }
    return sqrt(somme);
}

// Décomposition QR (Gram-Schmidt)
void decomposition_QR(matrice* A, matrice* Q, matrice* R) {
    int n = A->n, m = A->m;

    // Allocation pour Q et R
    *Q = matrice_nulle(n, n);
    *R = matrice_nulle(n, m);

    // Gram-Schmidt
    for (int j = 0; j < n; j++) {
        // Copier la colonne j de A dans Q
        for (int i = 0; i < n; i++) {
            Q->mat[i][j] = A->mat[i][j];
        }

        // Orthogonalisation
        for (int k = 0; k < j; k++) {
            long double dot_product = 0.0;
            for (int i = 0; i < n; i++) {
                dot_product += Q->mat[i][k] * A->mat[i][j];
            }
            R->mat[k][j] = dot_product;
            for (int i = 0; i < n; i++) {
                Q->mat[i][j] -= R->mat[k][j] * Q->mat[i][k];
            }
        }

        // Normalisation
        R->mat[j][j] = norme_vecteur_colonne(*Q, j);
        for (int i = 0; i < n; i++) {
            Q->mat[i][j] /= (float) R->mat[j][j];
        }
    }
}

// Fonction QR itérative pour la décomposition SVD après la bidiagonalisation
void qr_iteratif(matrice *A, matrice* S, matrice* V) {
    // Appliquer l'algorithme QR itératif à la matrice bidiagonale A pour obtenir les valeurs et vecteurs singuliers
    matrice B = *A;
    matrice Q, R;

    for (int i = 0; i < 20; i++) {  // Itérations arbitraires
        decomposition_QR(&B, &Q, &R);
        B = produit(R, Q);
    }

    // Résultat de la décomposition QR itérative
    *S = B;
    *V = transposee(R);
}

// Décomposition en valeurs singulières (SVD) (pour une matrice réelle)
void decomposition_SVD(matrice* A, matrice* U, matrice* S, matrice* V) {
    // Décomposer A pour obtenir U
    matrice Q_u, A1;
    decomposition_QR(A, &Q_u, &A1);
    *U = Q_u;
    
    // Calculer V et S en décomposant A1t
    matrice A1t = transposee(A1);
    matrice Q_v, R_s;
    qr_iteratif(&A1t, &Q_v, &R_s);

    // A = Q_u * A1 et A1t = Q_v * R_s 
    *S = R_s;
    *V = Q_v;

    free_matrice(A1t);
}

int main() {
    int n = 3, m = 3; // Exemple pour une matrice 3x3
    matrice A = matrice_nulle(n, m);

    // Remplir la matrice A avec des valeurs pour l'exemple
    A.mat[0][0] = 8; A.mat[0][1] = 1; A.mat[0][2] = -1;
    A.mat[1][0] = 2; A.mat[1][1] = 8; A.mat[1][2] = 1;
    A.mat[2][0] = 0; A.mat[2][1] = 3; A.mat[2][2] = 9;

    printf("Matrice A :\n");
    print_matrice(A);

    // Décomposition SVD
    matrice U, S, V;
    decomposition_SVD(&A, &U, &S, &V);

    printf("\nMatrice U (orthogonale) :\n");
    print_matrice(U);
    printf("\nMatrice S (diagonale des valeurs singulières) :\n");
    print_matrice(S);
    printf("\nMatrice V (orthogonale) :\n");
    print_matrice(V);

    // Reconstruction de A à partir de U, S, V
    matrice V_transpose = transposee(V);
    matrice recons_part = produit(U, S);
    matrice recons = produit(recons_part, V_transpose);

    printf("\nMatrice reconstruite (A = U * S * V^T) :\n");
    print_matrice(recons);

    // Libérer la mémoire des matrices temporaires
    free_matrice(recons_part);
    free_matrice(V_transpose);
    free_matrice(recons);

    // Libérer la mémoire
    free_matrice(A);
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);

    return 0;
}
