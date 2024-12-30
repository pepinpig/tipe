#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrice.h"
#define NUM_POINTS 4
#define TOTAL_POINTS 12

// Fonction pour initialiser des indices uniques aléatoires
void generate_unique_indices(int indices[], int num_indices, int max_value) {
    int chosen[TOTAL_POINTS] = {0};
    int count = 0;

    while (count < num_indices) {
        int index = rand() % max_value;
        if (!chosen[index]) {
            chosen[index] = 1;
            indices[count++] = index;
        }
    }
}

// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* V, int indices[]) {
    FILE *file = fopen(filename, "r");
    float u, v;
    float all_points[TOTAL_POINTS][2];

    // Lire tous les points du fichier
    for (int i = 0; i < TOTAL_POINTS; i++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        all_points[i][0] = u;
        all_points[i][1] = v;
    }

    // Sélectionner uniquement les points correspondant aux indices choisis
    for (int j = 0; j < NUM_POINTS; j++) {
        int index = indices[j];
        V->mat[3*j][0] = all_points[index][0];
        V->mat[3*j+1][0] = all_points[index][1];
        V->mat[3*j+2][0] = 1;
    }

    fclose(file);
}

// Fonction pour charger les points réels depuis un fichier
void load_points_reels(const char* filename, matrice* variables, int indices[]) {
    FILE *file = fopen(filename, "r");
    float X, Y, Z;
    float all_points[TOTAL_POINTS][3];

    // Lire tous les points du fichier
    for (int i = 0; i < TOTAL_POINTS; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        all_points[i][0] = X;
        all_points[i][1] = Y;
        all_points[i][2] = Z;
    }

    // Sélectionner uniquement les points correspondant aux indices choisis
    for (int i = 0; i < NUM_POINTS; i++) {
        int index = indices[i];
        for (int j = 0; j < 3; j++) {
            variables->mat[3*i+j][4*j] = all_points[index][0];
            variables->mat[3*i+j][4*j+1] = all_points[index][1];
            variables->mat[3*i+j][4*j+2] = 0;
            variables->mat[3*i+j][4*j+3] = 1;
        }
    }

    fclose(file);
}

int main() {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    // Générer 4 indices uniques aléatoires entre 0 et 11
    int indices[NUM_POINTS];
    generate_unique_indices(indices, NUM_POINTS, TOTAL_POINTS);

    matrice* variables = matrice_nulle_pointeur(3*NUM_POINTS, 3*4);
    matrice* V = matrice_nulle_pointeur(3*NUM_POINTS, 1);

    // Charger les points en utilisant les indices aléatoires
    load_points_images("points/points_img2D.txt", V, indices);
    load_points_reels("points/points_reel2D.txt", variables, indices);

    /*    print_matrice(*variables);
    print_matrice(*V);*/
    matrice* sol = least_squares_solution(variables, V);
    /*    print_matrice(*sol);*/
    matrice P = matrice_nulle(3, 4);
    P.mat[0][0] = sol->mat[0][0]; P.mat[0][1] = sol->mat[1][0]; P.mat[0][2] = sol->mat[2][0]; P.mat[0][3] = sol->mat[3][0];
    P.mat[1][0] = sol->mat[4][0]; P.mat[1][1] = sol->mat[5][0]; P.mat[1][2] = sol->mat[6][0]; P.mat[1][3] = sol->mat[7][0];
    P.mat[2][0] = sol->mat[8][0]; P.mat[2][1] = sol->mat[9][0]; P.mat[2][2] = sol->mat[10][0]; P.mat[2][3] = sol->mat[11][0];
    print_matrice(P);

    return 0;
}
