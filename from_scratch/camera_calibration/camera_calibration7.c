#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrice.h"
#define TOTAL_POINTS 12

// Fonction pour charger tous les points depuis un fichier
void load_all_points_images(const char* filename, matrice* V) {
    FILE *file = fopen(filename, "r");
    float u, v;

    for (int j = 0; j < TOTAL_POINTS; j++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        V->mat[3*j][0] = u;
        V->mat[3*j+1][0] = v;
        V->mat[3*j+2][0] = 1;
    }
    fclose(file);
}

// Fonction pour charger tous les points réels depuis un fichier
void load_all_points_reels(const char* filename, matrice* variables) {
    FILE *file = fopen(filename, "r");
    float X, Y, Z;

    for (int i = 0; i < TOTAL_POINTS; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        for (int j = 0; j < 3; j++) {
            variables->mat[3*i+j][4*j] = X;
            variables->mat[3*i+j][4*j+1] = Y;
            variables->mat[3*i+j][4*j+2] = 0;
            variables->mat[3*i+j][4*j+3] = 1;
        }
    }
    fclose(file);
}

// Fonction pour appliquer la méthode des moindres carrés
matrice* least_squares_solution(matrice* A, matrice* B) {
    // Calculer A^T * A
    matrice At = transposee(*A);
    matrice AtA = produit(At, *A);

    // Calculer A^T * B
    matrice AtB = produit(At, *B);

    // Résoudre le système AtA * X = AtB
    matrice* X = resolution_systeme(&AtA, &AtB);

    // Libérer la mémoire
    free_matrice(At);
    free_matrice(AtA);
    free_matrice(AtB);

    return X;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_de_l'image>\n", argv[0]);
        return 1;
    }

    // Récupérer le nom de l'image depuis les arguments
    const char* image_name = argv[1];

    // Construire le nom du fichier de points correspondant
    char points_image_file[256];
    snprintf(points_image_file, sizeof(points_image_file), "points/points_%s.txt", image_name);

    matrice* variables = matrice_nulle_pointeur(3 * TOTAL_POINTS, 3 * 4);
    matrice* V = matrice_nulle_pointeur(3 * TOTAL_POINTS, 1);

    // Charger tous les points
    load_all_points_images(points_image_file, V);
    load_all_points_reels("points/points_reel2D.txt", variables);

/*    print_matrice(*sol);*/
    matrice P = matrice_nulle(3, 4);
    P.mat[0][0] = sol->mat[0][0]; P.mat[0][1] = sol->mat[1][0]; P.mat[0][2] = sol->mat[2][0]; P.mat[0][3] = sol->mat[3][0];
    P.mat[1][0] = sol->mat[4][0]; P.mat[1][1] = sol->mat[5][0]; P.mat[1][2] = sol->mat[6][0]; P.mat[1][3] = sol->mat[7][0];
    P.mat[2][0] = sol->mat[8][0]; P.mat[2][1] = sol->mat[9][0]; P.mat[2][2] = sol->mat[10][0]; P.mat[2][3] = sol->mat[11][0];
    print_matrice(P);

    return 0;
}
