#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"  

#define MAX_ITER 1000  
#define TOLERANCE 1e-6  


void copie_colonne(matrice* dest, matrice source, int colonne_source, int colonne_dest) {
    for (int i = 0; i < source.n; ++i) {
        dest->mat[i][colonne_dest] = source.mat[i][colonne_source];
    }
}

// Effectue la décomposition QR de la matrice A en matrices Q et R
void decomposition_QR(matrice A, matrice* Q, matrice* R) {
    int n = A.n;
    int m = A.m;
    *Q = matrice_nulle(n, m);
    *R = matrice_nulle(m, m);

    for (int j = 0; j < m; ++j) {
        copie_colonne(Q, A, j, j);

        for (int i = 0; i < j; ++i) {
            long double r_ij = 0.0;
            for (int k = 0; k < n; ++k) {
                r_ij += Q->mat[k][i] * A.mat[k][j];
            }
            R->mat[i][j] = r_ij;
            
            for (int k = 0; k < n; ++k) {
                Q->mat[k][j] -= r_ij * Q->mat[k][i];
            }
        }

        // normalisation
        float norm = norme_vecteur(*Q, j);
        if (norm == 0) {
            fprintf(stderr, "Erreur : la matrice A est singulière ou proche d'être singulière.\n");
            exit(EXIT_FAILURE);
        }
        R->mat[j][j] = norm;
        for (int k = 0; k < n; ++k) {
            Q->mat[k][j] /= norm;
        }
    }
}



// calcule les valeurs propres et vecteurs propres de A
void QR_algorithm(matrice A, matrice* valeurs_propres, matrice* vecteurs_propres) {
    int n = A.n;
    matrice Q, R;
    
    // Copie de la matrice A pour les itérations
    matrice Ak = matrice_nulle(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Ak.mat[i][j] = A.mat[i][j];
        }
    }

    // Matrice pour accumuler les transformations Q à chaque itération
    *vecteurs_propres = matrice_identite(n);

    for (int iter = 0; iter < MAX_ITER; ++iter) {
        // Effectuer la décomposition QR
        decomposition_QR(Ak, &Q, &R);
        
        // Mettre à jour Ak = R * Q
        Ak = produit(R, Q);
        
        // Accumuler les transformations Q pour construire les vecteurs propres
        *vecteurs_propres = produit(*vecteurs_propres, Q);

        // Vérifier la convergence en utilisant la somme des valeurs hors diagonale
        float somme_hors_diagonale = 0.0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    somme_hors_diagonale += fabs(Ak.mat[i][j]);
                }
            }
        }

/*        if (somme_hors_diagonale < TOLERANCE) {
            break;
        }*/

        // Libération de la mémoire pour Q et R après chaque itération
        free_matrice(Q);
        free_matrice(R);
    }

    // Stocker les valeurs propres (diagonale de Ak)
    *valeurs_propres = matrice_nulle(n, 1);
    for (int i = 0; i < n; ++i) {
        valeurs_propres->mat[i][0] = Ak.mat[i][i];
    }

    // Libération de la mémoire
    free_matrice(Ak);
}
/*
int main() {
    // Exemple d'utilisation
    int n = 3;
    matrice A = matrice_nulle(n, n);
    
    // Remplissage de la matrice A avec des valeurs d'exemple
    A.mat[0][0] = 12; A.mat[0][1] = -51; A.mat[0][2] = 4;
    A.mat[1][0] = 6;  A.mat[1][1] = 167; A.mat[1][2] = -68;
    A.mat[2][0] = -4; A.mat[2][1] = 24;  A.mat[2][2] = -41;
    
    // Initialisation des matrices pour stocker les valeurs et vecteurs propres
    matrice valeurs_propres, vecteurs_propres;

    // Calcul des valeurs et vecteurs propres avec l'algorithme QR
    QR_algorithm(A, &valeurs_propres, &vecteurs_propres);

    // Affichage des résultats
    printf("Valeurs propres :\n");
    print_matrice(valeurs_propres);
    
    printf("\nVecteurs propres (colonnes de Q) :\n");
    print_matrice(vecteurs_propres);
    
    // Libération de la mémoire
    free_matrice(A);
    free_matrice(valeurs_propres);
    free_matrice(vecteurs_propres);

    return 0;
}
*/