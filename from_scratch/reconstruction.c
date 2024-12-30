#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "SVD.h"
#define N 6

// Fonction pour résoudre le système A*X=Y à l'aide de SVD
matrice resolution_systeme_SVD(matrice* A, matrice* Y) {
    // Dimensions de la matrice A
    int n = A->n;
    int m = A->m;

    // Matrices pour la décomposition SVD
    matrice* U = matrice_nulle_pointeur(n, n);   // Matrice orthogonale U (n x n)
    matrice* S = matrice_nulle_pointeur(n, m);   // Matrice diagonale Σ (n x m)
    matrice* V = matrice_nulle_pointeur(m, m);  // Matrice orthogonale V^T (m x m)

    // Effectuer la décomposition SVD
    qr_algorithm_SVD(A, U, S, V);

    // Calcul de Σ^+ (pseudo-inverse de la matrice Σ)
    matrice* S_plus = matrice_nulle_pointeur(m, n);  // Σ^+ (m x n)
    for (int i = 0; i < n && i < m; i++) {
        long double sigma = S->mat[i][i];
        if (fabs(sigma) > 1e-10) {  // Éviter les divisions par zéro
            S_plus->mat[i][i] = 1.0 / sigma;
        }
    }

    // Calcul de A^+ = V * Σ^+ * U^T

    matrice V_S_plus = produit(*V, *S_plus); // Stocke V * Σ^+
    matrice U_transpose = transposee(*U);
    matrice A_plus = produit(V_S_plus, U_transpose);

    // Calcul de la solution X = A^+ * Y
    matrice solution = produit(A_plus, *Y);
    // Libération des matrices
    free_matrice(*U);
    free_matrice(*S);
    free_matrice(*V);
    free_matrice(*S_plus);
    free_matrice(V_S_plus);
    free_matrice(U_transpose);
    free_matrice(A_plus);

    // Retourner la solution
    return solution;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_de_l'image>\n", argv[0]);
        return 1;
    }
    long double* X = calloc(N, sizeof(long double));
    long double* Y = calloc(N, sizeof(long double));
    long double* Z = calloc(N, sizeof(long double));
    long double* u = calloc(N, sizeof(long double));
    long double* v = calloc(N, sizeof(long double));

    // Récupérer le nom de l'image depuis les arguments
    const char* image_name = argv[1];
    char points_image_file[256];
    char points_reel_file[256];
    snprintf(points_image_file, sizeof(points_image_file), "points/points_%s.txt", image_name);
    snprintf(points_reel_file, sizeof(points_reel_file), "points/points_reel%s.txt", image_name);

    printf("Lecture des points ...\n");
    load_all_points_images(points_image_file, u, v);
    load_all_points_reels(points_reel_file, X, Y, Z);

    char p_file[256];
    snprintf(p_file, sizeof(p_file), "P-%s.txt", image_name);
    printf("Lecture P ...\n");
    matrice P = lire_matrice(p_file);
    print_matrice(P);

    printf("Reconstruction ...\n");

    // Préparer le fichier de sortie
    char output_file[256];
    snprintf(output_file, sizeof(output_file), "points/points_reconstruit%s.txt", image_name);
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        matrice point = matrice_nulle(3, 1);
        point.mat[0][0] = u[i];
        point.mat[1][0] = v[i];
        point.mat[2][0] = 1.0;
        printf("calcul pour le point %d : \n", i);
        print_matrice(point);
        matrice result = resolution_systeme_SVD(&P, &point);

        // Normalisation des coordonnées
        for (int j = 0; j < 4; ++j) {
            result.mat[j][0] /= result.mat[3][0];  // Normalisation en divisant par la dernière composante
        }
        printf("Coordonnées : %Lf %Lf %Lf\n", result.mat[0][0], result.mat[1][0], result.mat[2][0]);
        // Écriture des coordonnées dans le fichier
        fprintf(output, "%Lf %Lf %Lf\n", result.mat[0][0], result.mat[1][0], result.mat[2][0]);

        // Libération des matrices allouées
        free_matrice(point);
        free_matrice(result);
    }

    fclose(output);
    printf("Coordonnées reconstruites écrites dans : %s\n", output_file);

    // Libération des tableaux
    free(X);
    free(Y);
    free(Z);
    free(u);
    free(v);

    return 0;
}
