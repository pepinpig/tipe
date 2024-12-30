#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "SVD.h"
#define N 6  // Nombre de points



matrice* contruction_A(long double* X, long double* Y, long double* Z, long double* u, long double* v) {
    matrice* A = matrice_nulle_pointeur(2 * N, 12); // 12 inconnues (p_11 à p_34)

    for (int i = 0; i < N; i++) {
        int row1 = 2 * i;
        int row2 = 2 * i + 1;
        // lambda_i * u_i = ...
        A->mat[row1][0] = X[i];
        A->mat[row1][1] = Y[i];
        A->mat[row1][2] = Z[i];
        A->mat[row1][3] = 1;
        A->mat[row1][8] = -X[i]* u[i];
        A->mat[row1][9] = -Y[i]* u[i];
        A->mat[row1][10] = -Z[i]* u[i];
        A->mat[row1][11] = -u[i];

        // lambda_i * v_i = ...
        A->mat[row2][4] = X[i];
        A->mat[row2][5] = Y[i];
        A->mat[row2][6] = Z[i];
        A->mat[row2][7] = 1;
        A->mat[row2][8] = -X[i]* v[i];
        A->mat[row2][9] = -Y[i]* v[i];
        A->mat[row2][10] = -Z[i]* v[i];
        A->mat[row2][11] = -v[i];

    }
    return A;
    // Résoudre le système linéaire A * P = B pour obtenir les paramètres
    // Libération de la mémoire allouée pour les matrices A et B
/*    free_matrice(A);
    free_matrice(B);*/
}

void camera_calibration_resolution(matrice* P, matrice* A) {
    // Étape 1 : Calcul de la décomposition SVD de A
    matrice U, S, V; // Matrices pour la décomposition SVD
    qr_algorithm_SVD(A, &U, &S, &V);

    // Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
    long double min_sigma = S.mat[S.n - 1][S.m - 1];
    int index_min = S.n - 1;
/*    for (int i = 0; i < S.n && i < S.m; i++) {
        if (S.mat[i][i] < min_sigma) {
            min_sigma = S.mat[i][i];
            index_min = i;
        }
    }*/
    printf("%d\n",index_min );
    printf("%d\n",V.m );
    printf("%d\n",V.n);
    matrice p = matrice_nulle(V.m, 1);
    print_matrice(p);
    print_matrice(V);
    for (int i = 0; i < V.m; i++) {
        p.mat[i][0] = V.mat[i][index_min];
        printf("%Lf\n",p.mat[i][0]);
    }

    // Libérer U, S 
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);
    
    // Étape 3 : Construire la matrice P

    for (int i = 0; i < 4; i++) { // Ligne 1
        P->mat[0][i] = p.mat[i][0];
    }
    for (int i = 4; i < 8; i++) { // Ligne 2
        P->mat[1][i-4] = p.mat[i][0];
    }
    for (int i = 8; i < 12; i++) { // Ligne 3
        P->mat[2][i-8] = p.mat[i][0];
    }
    printf("P\n");
    print_matrice(*P);
    // Libération du vecteur p
    free_matrice(p);

    // Étape 4 : Calcul de la décomposition SVD de P
    matrice U2,C, S2, V2;
    qr_algorithm_SVD(P, &U2, &S2, &V2);

    // Extraction du vecteur singulier droit associé à la plus petite valeur singulière
    index_min = fmin(P->n - 1,P->m-1);

    printf("%d\n", index_min );
    print_matrice(V2);
    matrice C_approx = matrice_nulle(V2.m, 1);
    for (int i = 0; i < V2.m; i++) {
        C_approx.mat[i][0] = V2.mat[i][index_min];
    }
    print_matrice(C_approx);

    // Normalisation de C pour que le dernier élément soit 1
    long double scale = C_approx.mat[C_approx.n - 1][0];
    for (int i = 0; i < C_approx.n; i++) {
        C_approx.mat[i][0] /= scale;
    }

   // Étape 5 : QR factorisation de P(1:3, 1:3)^-1
    matrice* P_extract=matrice_nulle_pointeur(3,3);
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            P_extract->mat[i][j]=P->mat[i][j];
        }
    }
    matrice* P_inv = inverser_matrice(P_extract);
    print_matrice(*P_inv);
    matrice Q, R;
    decomposition_QR(P_inv, &Q, &R);

    // Étape 6 : Calcul de K, R et T
    matrice* K = inverser_matrice(&R);
    matrice* R_inv = inverser_matrice(&Q);
    multiplication_scalaire(*K, 1.0 / K->mat[2][2]); // Normalisation de K(3,3) = 1

    matrice T = produit(*K, *R_inv);
    multiplication_scalaire(T, -1); // T = -RC
    // Affichage des résultats
    printf("Matrice K :\n");
    print_matrice(*K);
    printf("Matrice R :\n");
    print_matrice(*R_inv);
    printf("Matrice T :\n");
    print_matrice(T);

    // Libération de la mémoire
/*    free_matrice(P_inv);
    free_matrice(Q);
    free_matrice(R);
    free_matrice(K);
    free_matrice(R_inv);
    free_matrice(T);
    free_matrice(C_approx);
    free_matrice(U);
    free_matrice(S2);
    free_matrice(V2);*/
}


int main(int argc, char* argv[]) {
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
    char* image_name = argv[1];
    char points_image_file[256];
    char points_reel_file[256];
    snprintf(points_image_file, sizeof(points_image_file), "points/points_%s.txt", image_name);
    snprintf(points_reel_file, sizeof(points_reel_file), "points/points_reel%s.txt", image_name);

    load_all_points_images(points_image_file, u, v);
    load_all_points_reels(points_reel_file, X, Y, Z);

    matrice* A = contruction_A(X, Y, Z, u, v);
    printf("A:\n");
    print_matrice(*A);
    char fn[100];
    nom_fichier(fn, "A", image_name);
    save_matrice_to_file(A, fn);
    matrice P = matrice_nulle(3, 4);
    camera_calibration_resolution(&P, A);
    printf("P:\n");
    print_matrice(P);
    nom_fichier(fn, "P", image_name);
    save_matrice_to_file(&P, fn);

    // Ajouter d'autres matrices (K, T, ...) si nécessaire
    // Par exemple, si vous calculez une matrice K :
    /*
    matrice* K = calcul_K(&P);
    save_matrix_to_file(K, "K", image_name);
    free_matrice(K);
    */

    // Libérer la mémoire
    free(X);
    free(Y);
    free(Z);
    free(u);
    free(v);
    free_matrice(*A);

    return 0;
}
