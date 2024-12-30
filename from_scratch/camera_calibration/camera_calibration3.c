#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#define NUM_POINTS 6


// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float u; float v;
    for (int j =0; j < 5; j++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        variables->mat[3*j][12+j]=u;
        variables->mat[3*j+1][12+j]=v;
        variables->mat[3*j+2][12+j]=1;
    }
        variables->mat[3*5][12+5]=u;
        variables->mat[3*5+1][12+5]=v;
        variables->mat[3*5+2][12+5]=1;
    fclose(file);
}
// Fonction pour charger les points depuis un fichier
void load_points_reels(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float X; float Y; float Z;
    for (int i =0; i < 5; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        for (int j =0; j < 3; j++) {
            variables->mat[3*i+j][4*j]=X;
            variables->mat[3*i+j][4*j+1]=Y;
            variables->mat[3*i+j][4*j+2]=Z;
            variables->mat[3*i+j][4*j+3]=1;
            variables->mat[3*5+j][4*j]=X;
            variables->mat[3*5+j][4*j+1]=Y;
            variables->mat[3*5+j][4*j+2]=Z;
            variables->mat[3*5+j][4*j+3]=1;
        }
    }
    fclose(file);
}

int main() {
    matrice* variables=matrice_nulle_pointeur(3*6,3*6);
    matrice* zero=matrice_nulle_pointeur(18,1);
    for (int i = 0; i < 17; ++i)
    {
        zero->mat[i][0]=0.1;
    }
    load_points_images("points/points_img1.txt", variables,0);
    load_points_reels("points/points_reel.txt", variables,0);
    print_matrice(*variables);
    resolution_systeme_print(variables,zero);
    /*print_matrice(*sol);*/
    matrice P=matrice_nulle(3,4);
    P.mat[0][0]=sol->mat[0][0] ;P.mat[0][1]=sol->mat[1][0] ;P.mat[0][2]=sol->mat[2][0] ;P.mat[0][3]=sol->mat[3][0] ;
    P.mat[1][0]=sol->mat[4][0] ;P.mat[1][1]=sol->mat[5][0] ;P.mat[1][2]=sol->mat[6][0] ;P.mat[1][3]=sol->mat[7][0] ;
    P.mat[2][0]=sol->mat[8][0] ;P.mat[2][1]=sol->mat[9][0] ;P.mat[2][2]=sol->mat[10][0] ;P.mat[2][3]=sol->mat[11][0] ;
    print_matrice(P);
    return 0;
}
