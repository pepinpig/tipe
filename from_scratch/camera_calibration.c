#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include "SVD.h"
#define NUM_POINTS 5

// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* variables) {
    FILE *file = fopen(filename, "r");
    float u, v;

    for (int j = 0; j < NUM_POINTS; j++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        variables->mat[2*j][6] = -u;
        variables->mat[2*j][7] = -u;
        variables->mat[2*j][8] = -u;
        variables->mat[2*j][9] = -u;
        variables->mat[2*j+1][10] = -v;
        variables->mat[2*j+1][11] = -v;
        variables->mat[2*j+1][12] = -v;
        variables->mat[2*j+1][13] = -v;
    }
    fclose(file);
}

// Fonction pour charger les points réels depuis un fichier
void load_points_reels(const char* filename, matrice* variables) {
    FILE *file = fopen(filename, "r");
    float X, Y, Z;

    for (int i = 0; i < NUM_POINTS; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        for (int j = 0; j < 2; j++) {
            variables->mat[2*i+j][3*j] = X;
            variables->mat[2*i+j][3*j+1] = Y;
            variables->mat[2*i+j][3*j+2] = Z;
            variables->mat[2*i+j][8] = (variables->mat[2*i+j][8]) * X;
            variables->mat[2*i+j][9] = (variables->mat[2*i+j][9]) * Y;
            variables->mat[2*i+j][10] = (variables->mat[2*i+j][10]) * Z;   
        }
    }
    fclose(file);
}


void camera_calibration_resolution(matrice* P, matrice* A) {
    // Étape 1 : Calcul de la décomposition SVD de A
    matrice U, S, V; // Matrices pour la décomposition SVD
    /*qr_algorithm_SVD(A, &U, &S, &V);*/
    qr_algorithm(A,&S);
/*
    // Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
    long double min_sigma = S.mat[S.n - 1][S.m - 1];
    int index_min = S.n - 1;
    for (int i = 0; i < S.n && i < S.m; i++) {
        if (S.mat[i][i] < min_sigma) {
            min_sigma = S.mat[i][i];
            index_min = i;
        }
    }
    matrice p = matrice_nulle(V.m, 1);
    for (int i = 0; i < V.m; i++) {
        p.mat[i][0] = V.mat[i][index_min];
    }

    // Libérer U, S car ils ne sont plus nécessaires
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);

    // Étape 2 : Normalisation du vecteur p (composantes 9:11)
    long double norm = 0;
    for (int i = 8; i <= 10; i++) { // Indices 9:11 en base 0
        norm += p.mat[i][0] * p.mat[i][0];
    }
    norm = sqrt(norm);
    for (int i = 8; i <= 10; i++) {
        p.mat[i][0] /= norm;
    }

    // Étape 3 : Construire la matrice P
    for (int i = 0; i < 4; i++) { // Ligne 1
        P->mat[0][i] = p.mat[i][0];
    }
    for (int i = 4; i < 8; i++) { // Ligne 2
        P->mat[1][i - 4] = p.mat[i][0];
    }
    for (int i = 8; i < 12; i++) { // Ligne 3
        P->mat[2][i - 8] = p.mat[i][0];
    }

    // Libération du vecteur p
    free_matrice(p);

    // Étape 4 : Calcul de la décomposition SVD de P
    matrice C, S2, V2;
    decomposition_SVD(*P, &U, &S2, &V2);

    // Extraction du vecteur singulier droit associé à la plus petite valeur singulière
    min_sigma = S2.mat[S2.n - 1][S2.m - 1];
    index_min = S2.n - 1;
    for (int i = 0; i < S2.n && i < S2.m; i++) {
        if (S2.mat[i][i] < min_sigma) {
            min_sigma = S2.mat[i][i];
            index_min = i;
        }
    }
    matrice C_approx = matrice_nulle(V2.m, 1);
    for (int i = 0; i < V2.m; i++) {
        C_approx.mat[i][0] = V2.mat[i][index_min];
    }

    // Normalisation de C pour que le dernier élément soit 1
    long double scale = C_approx.mat[C_approx.n - 1][0];
    for (int i = 0; i < C_approx.n; i++) {
        C_approx.mat[i][0] /= scale;
    }

    // Étape 5 : QR factorisation de P(1:3, 1:3)^-1
    matrice P_inv = inverser_matrice(P);
    matrice Q, R;
    decomposition_QR(P_inv, &Q, &R);

    // Étape 6 : Calcul de K, R et T
    matrice K = inverser_matrice(&R);
    matrice R_inv = inverser_matrice(&Q);
    multiplication_scalaire(K, 1.0 / K.mat[2][2]); // Normalisation de K(3,3) = 1

    matrice T = produit(K, R_inv);
    multiplication_scalaire(T, -1); // T = -RC

    // Affichage des résultats
    printf("Matrice K :\n");
    print_matrice(K);
    printf("Matrice R :\n");
    print_matrice(R_inv);
    printf("Matrice T :\n");
    print_matrice(T);

    // Libération de la mémoire
    free_matrice(P_inv);
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

int main() {
    // Créer des matrices avec le bon nombre de points
    matrice* variables = matrice_nulle_pointeur(2 * NUM_POINTS, (2 * 3) + 3);

    // Charger les points
    load_points_images("points/points_img1.txt", variables);
    load_points_reels("points/points_reel.txt", variables);
    matrice* sol = matrice_nulle_pointeur(12, 1);
    print_matrice(*variables);


    // Récupérer P
    matrice P = matrice_nulle(3, 4);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            P.mat[i][j] = sol->mat[i * 4 + j][0]; 
        }
    }
    
    print_matrice(P);

    // Libération de la mémoire

    return 0;
}

/*int main() {
    // Créer des matrices avec le bon nombre de points
    matrice* variables = matrice_nulle_pointeur(2 * NUM_POINTS, (2 * 3) + 3);
    matrice* zero = matrice_nulle_pointeur(2 * NUM_POINTS, 1);

    // Charger les points
    load_points_images("points/points_img1.txt", variables);
    load_points_reels("points/points_reel.txt", variables);
    
    print_matrice(*variables);

    matrice* sol = resolution_systeme(variables, zero);

    // Récupérer P
    matrice P = matrice_nulle(3, 4);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            P.mat[i][j] = sol->mat[i * 4 + j][0]; 
        }
    }
    
    print_matrice(P);

    // Libération de la mémoire
    free_matrice(variables);
    free_matrice(zero);
    free_matrice(sol);

    return 0;
}*/
