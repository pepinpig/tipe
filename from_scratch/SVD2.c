#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "matrice.h"
#include "SVD.h"
#include "manipulation_fichiers.h"

#define precision 1e-30
#define max_iters 10000

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

    *Q = matrice_nulle(n, n);
    *R = matrice_nulle(n, m);

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            Q->mat[i][j] = A->mat[i][j];
        }

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

        R->mat[j][j] = norme_vecteur_colonne(*Q, j);
        if (R->mat[j][j] > precision) {
            for (int i = 0; i < n; i++) {
                Q->mat[i][j] /= R->mat[j][j];
            }
        }
    }
}

// Ajout de la fonction d'itération inverse
long double inverse_iteration_min(matrice A, long double seuil) {
    assert(A.n == A.m); // La matrice doit être carrée
    int n = A.n;
    matrice x = matrice_nulle(n, 1);
    matrice y = matrice_nulle(n, 1);
    matrice z = matrice_nulle(n, 1);
    for (int i = 0; i < n; i++) {
        x.mat[i][0] = 1.0; // Initialisation arbitraire
    }

    long double lambda = 0.0;
    long double lambda_prev = 1.0;
    int iter = 0;

    while (fabs(lambda - lambda_prev) > seuil && iter < max_iters) {
        lambda_prev = lambda;

        // Résolution de Az = x (z = A⁻¹x)
        y = resolution_gauss(A, x);

        // Normalisation de y
        long double norm_y = norme_vecteur_colonne(y, 0);
        for (int i = 0; i < n; i++) {
            z.mat[i][0] = y.mat[i][0] / norm_y;
        }

        // Calcul de λ = (zᵀ * A * z) / (zᵀ * z)
        matrice Az = produit(A, z);
        matrice zT = transposee(z);
        matrice num = produit(zT, Az);
        matrice den = produit(zT, z);

        lambda = num.mat[0][0] / den.mat[0][0];

        // Mettre à jour x pour la prochaine itération
        for (int i = 0; i < n; i++) {
            x.mat[i][0] = z.mat[i][0];
        }

        // Libération des matrices temporaires
        free_matrice(y);
        free_matrice(Az);
        free_matrice(zT);
        free_matrice(num);
        free_matrice(den);

        iter++;
    }

    printf("Convergence atteinte après %d itérations. Valeur propre minimale : %Lf\n", iter, lambda);

    // Libération de la mémoire
    free_matrice(x);
    free_matrice(z);

    return lambda;
}

int main() {
    int n = 4;
    matrice A = matrice_nulle(n, n);

    // Exemple de matrice
    A.mat[0][0] = -1; A.mat[0][1] = 0; A.mat[0][2] = 0; A.mat[0][3] = 0;
    A.mat[1][0] = 0; A.mat[1][1] = -1; A.mat[1][2] = 0; A.mat[1][3] = 0;
    A.mat[2][0] = -1; A.mat[2][1] = 0; A.mat[2][2] = 0.1; A.mat[2][3] = 0.5;
    A.mat[3][0] = 0; A.mat[3][1] = -1; A.mat[3][2] = 0; A.mat[3][3] = 0;

    printf("Matrice A :\n");
    print_matrice(A);

    // Calcul de la valeur propre minimale
    long double valeur_propre_min = inverse_iteration_min(A, precision);
    printf("Valeur propre minimale (méthode de l'itération inverse) : %Lf\n", valeur_propre_min);

    // Libération de la mémoire
    free_matrice(A);

    return 0;
}
