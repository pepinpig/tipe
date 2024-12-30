#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#define NUM_POINTS 6


// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float u; float v;
    for (int j =0; j < NUM_POINTS; j++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        variables->mat[3*j][12+j]=u;
        variables->mat[3*j+1][12+j]=v;
        variables->mat[3*j+2][12+j]=1;
    }
    fclose(file);
}
// Fonction pour charger les points depuis un fichier
void load_points_reels(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float X; float Y; float Z;
    for (int i =0; i < NUM_POINTS; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        for (int j =0; j < 3; j++) {
            variables->mat[3*i+j][4*j]=X;
            variables->mat[3*i+j][4*j+1]=Y;
            variables->mat[3*i+j][4*j+2]=Z;
            variables->mat[3*i+j][4*j+3]=1;
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

    matrice* variables=matrice_nulle_pointeur(3*NUM_POINTS,3*4+NUM_POINTS);
    matrice* zero=matrice_nulle_pointeur(3*NUM_POINTS,1);
    load_points_images(points_image_file, variables,0);
    load_points_reels("points/points_reel2D.txt", variables,0);
    print_matrice(*variables);
/*    matrice* sol=resolution_systeme(variables,zero);*/
    matrice* sol = least_squares_solution(variables, zero);
    /*print_matrice(*sol);*/
    matrice P=matrice_nulle(3,4);
    P.mat[0][0]=sol->mat[0][0] ;P.mat[0][1]=sol->mat[1][0] ;P.mat[0][2]=sol->mat[2][0] ;P.mat[0][3]=sol->mat[3][0] ;
    P.mat[1][0]=sol->mat[4][0] ;P.mat[1][1]=sol->mat[5][0] ;P.mat[1][2]=sol->mat[6][0] ;P.mat[1][3]=sol->mat[7][0] ;
    P.mat[2][0]=sol->mat[8][0] ;P.mat[2][1]=sol->mat[9][0] ;P.mat[2][2]=sol->mat[10][0] ;P.mat[2][3]=sol->mat[11][0] ;
    print_matrice(P);
    return 0;
}
