#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "SVD.h"
#include "N.h"

matrice resolution_systeme_SVD(matrice* A, matrice* Y) {
    int n = A->n;
    int m = A->m;

    matrice* U = matrice_nulle_pointeur(n, n);
    matrice* S = matrice_nulle_pointeur(n, m);
    matrice* V = matrice_nulle_pointeur(m, m);

    qr_algorithm_SVD(A, U, S, V);

    matrice* S_plus = matrice_nulle_pointeur(m, n);
    for (int i = 0; i < n && i < m; i++) {
        long double sigma = S->mat[i][i];
        if (fabs(sigma) > 1e-10) {
            S_plus->mat[i][i] = 1.0 / sigma;
        }
    }

    matrice V_S_plus = produit(*V, *S_plus);
    matrice U_transpose = transposee(*U);
    matrice A_plus = produit(V_S_plus, U_transpose);

    matrice solution = produit(A_plus, *Y);

    free_matrice(*U);
    free_matrice(*S);
    free_matrice(*V);
    free_matrice(*S_plus);
    free_matrice(V_S_plus);
    free_matrice(U_transpose);
    free_matrice(A_plus);

    return solution;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2> ... <nom_imageN>\n", argv[0]);
        return 1;
    }

    int num_images = argc - 1;
    long double** X = malloc(num_images * sizeof(long double*));
    long double** Y = malloc(num_images * sizeof(long double*));
    long double** Z = malloc(num_images * sizeof(long double*));
    long double** u = malloc(num_images * sizeof(long double*));
    long double** v = malloc(num_images * sizeof(long double*));

    matrice** P = malloc(num_images * sizeof(matrice*));

    for (int img = 0; img < num_images; ++img) {
        X[img] = calloc(N, sizeof(long double));
        Y[img] = calloc(N, sizeof(long double));
        Z[img] = calloc(N, sizeof(long double));
        u[img] = calloc(N, sizeof(long double));
        v[img] = calloc(N, sizeof(long double));

        char points_image_file[256], points_reel_file[256], p_file[256];
        snprintf(points_image_file, sizeof(points_image_file), "points/donnees/points_%s.txt", argv[img + 1]);
        snprintf(points_reel_file, sizeof(points_reel_file), "points/donnees/points_reels%s.txt", argv[img + 1]);
        nom_fichier(p_file, "P", argv[img + 1]);

        load_all_points_images(points_image_file, u[img], v[img]);
        load_all_points_reels(points_reel_file, X[img], Y[img], Z[img]);

        P[img] = matrice_nulle_pointeur(3, 4);
        read_matrice_from_file(P[img], p_file);
    }

    char output_file[256];
    snprintf(output_file, sizeof(output_file), "points/donnees/points_reconstruits.txt");
    FILE* output = fopen(output_file, "w");
    assert(output != NULL);

    printf("Reconstruction des points 3D ...\n");
    for (int i = 0; i < N; ++i) {
        matrice A = matrice_nulle(num_images * 2, 4);

        for (int img = 0; img < num_images; ++img) {
            for (int j = 0; j < 4; ++j) {
                A.mat[2 * img][j] = u[img][i] * P[img]->mat[2][j] - P[img]->mat[0][j];
                A.mat[2 * img + 1][j] = v[img][i] * P[img]->mat[2][j] - P[img]->mat[1][j];
            }
        }

        matrice U, S, V;
        qr_algorithm_SVD(&A, &U, &S, &V);

        long double min_sigma = S.mat[S.n - 1][S.m - 1];
        int index_min = S.n - 1;
        matrice p = matrice_nulle(V.m, 1);
        for (int k = 0; k < V.m; k++) {
            p.mat[k][0] = V.mat[k][index_min];
        }

        for (int k = 0; k < 3; ++k) {
            p.mat[k][0] /= p.mat[3][0];
        }

        fprintf(output, "%Lf %Lf %Lf\n", p.mat[0][0], p.mat[1][0], p.mat[2][0]);

        free_matrice(U);
        free_matrice(S);
        free_matrice(V);
        free_matrice(p);
    }

    fclose(output);

    for (int img = 0; img < num_images; ++img) {
        free(X[img]);
        free(Y[img]);
        free(Z[img]);
        free(u[img]);
        free(v[img]);
        free_matrice(*P[img]);
    }

    free(X);
    free(Y);
    free(Z);
    free(u);
    free(v);
    free(P);

    printf("Points reconstruits écrits dans : %s\n", output_file);
    return 0;
}
