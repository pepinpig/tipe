#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "SVD.h"
#define N 6  // Nombre de points


// Fonction pour charger les points depuis un fichier
void load_all_points_images(const char* filename, float* u, float*v) {
    FILE *file = fopen(filename, "r");
    float ui, vi;
    for (int i = 0; i < 6; i++) {
        fscanf(file, "%Lf %Lf", &ui, &vi);
        u[i]=ui;
        printf("%Lf\n",ui);
        v[i]=vi;
    }
    fclose(file);
}

// Fonction pour charger les points réels depuis un fichier
void load_all_points_reels(const char* filename, float* X, float* Y, float* Z) {
    FILE *file = fopen(filename, "r");
    float Xi, Yi, Zi;
    for (int i = 0; i < 6; i++) {
        fscanf(file, "%Lf %Lf %Lf", &Xi, &Yi, &Zi);
        X[i]=Xi;
        Y[i]=Yi;
        Z[i]=Zi;
    }
    fclose(file);
}
// Fonction pour résoudre le système en utilisant une matrice augmentée
void resoudre_systeme_parametre(float* X, float* Y, float* Z, float* u, float* v, matrice* solution) {
    matrice* A = matrice_nulle_pointeur(2 * N, 12); // 12 inconnues (p_11 à p_34)
    matrice* B = matrice_nulle_pointeur(2 * N, 1);  // Vecteur de termes constants

    // Remplissage des matrices A et B
    for (int i = 0; i < N; i++) {
        int row1 = 2 * i;
        int row2 = 2 * i + 1;

        // Remplir A et B pour lambda_i * u_i = ...
        A->mat[row1][0] = X[i];
        A->mat[row1][1] = Y[i];
        A->mat[row1][2] = Z[i];
        A->mat[row1][3] = 1;
        A->mat[row1][8] = -X[i]* u[i];
        A->mat[row1][9] = -Y[i]* u[i];
        A->mat[row1][10] = -Z[i]* u[i];
        A->mat[row1][11] = -u[i];

        // Remplir A et B pour lambda_i * v_i = ...
        A->mat[row2][4] = X[i];
        A->mat[row2][5] = Y[i];
        A->mat[row2][6] = Z[i];
        A->mat[row2][7] = 1;
        A->mat[row2][8] = -X[i]* v[i];
        A->mat[row2][9] = -Y[i]* v[i];
        A->mat[row2][10] = -Z[i]* v[i];
        A->mat[row2][11] = -v[i];

    }
    A->mat[11][0] = 0;
    A->mat[11][1] = 0;
    A->mat[11][2] = 0;
    A->mat[11][3] = 0;
    A->mat[11][8] = 0;
    A->mat[11][9] = 0;
    A->mat[11][10] = 0;
    A->mat[11][11] = 1;
    B->mat[11][0]=1;

    // Résoudre le système linéaire A * P = B pour obtenir les paramètres
    *solution = *resolution_systeme_print(A, B);

    // Libération de la mémoire allouée pour les matrices A et B
/*    free_matrice(A);
    free_matrice(B);*/
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_de_l'image>\n", argv[0]);
        return 1;
    }
    float* X = calloc(N,sizeof(float));
    float* Y = calloc(N,sizeof(float));
    float* Z = calloc(N,sizeof(float));
    float* u = calloc(N,sizeof(float));
    float* v = calloc(N,sizeof(float));
    // Récupérer le nom de l'image depuis les arguments
    const char* image_name = argv[1];
    char points_image_file[256];
    char points_reel_file[256];
    snprintf(points_image_file, sizeof(points_image_file), "points/points_%s.txt", image_name);
    snprintf(points_reel_file, sizeof(points_image_file), "points/points_reel%s.txt", image_name);
    load_all_points_images(points_image_file, u,v);
    load_all_points_reels(points_reel_file, X,Y,Z);

    // Matrice solution pour stocker les paramètres p_11 à p_34
    matrice solution = matrice_nulle(12, 1);

    // Résoudre le système
    resoudre_systeme_parametre(X, Y, Z, u, v, &solution);

    // Afficher les résultats
/*    printf("Les paramètres du système sont :\n");
    for (int i = 0; i < 12; i++) {
        printf("p_%d = %Lf\n", i + 1, solution.mat[i][0]);
    }*/
    matrice* P = matrice_nulle_pointeur(3, 4);
    P->mat[0][0] = solution.mat[0][0]; P->mat[0][1] = solution.mat[1][0]; P->mat[0][2] = solution.mat[2][0]; P->mat[0][3] = solution.mat[3][0];
    P->mat[1][0] = solution.mat[4][0]; P->mat[1][1] = solution.mat[5][0]; P->mat[1][2] = solution.mat[6][0]; P->mat[1][3] = solution.mat[7][0];
    P->mat[2][0] = solution.mat[8][0]; P->mat[2][1] = solution.mat[9][0]; P->mat[2][2] = solution.mat[10][0]; P->mat[2][3] = solution.mat[11][0];
    matrice* debP = matrice_nulle_pointeur(3, 3);
    debP->mat[0][0] = solution.mat[0][0]; debP->mat[0][1] = solution.mat[1][0]; debP->mat[0][2] = solution.mat[2][0];
    debP->mat[1][0] = solution.mat[4][0]; debP->mat[1][1] = solution.mat[5][0]; debP->mat[1][2] = solution.mat[6][0];
    debP->mat[2][0] = solution.mat[8][0]; debP->mat[2][1] = solution.mat[9][0]; debP->mat[2][2] = solution.mat[10][0]; 
    printf("Matrice P:\n");
    print_matrice(*debP);
    matrice R, Q;
    decomposition_RQ(debP, &R, &Q);
    printf("Matrice R:\n");
    print_matrice(R);
    printf("\nMatrice Q:\n");
    print_matrice(Q);
    return 0;
}