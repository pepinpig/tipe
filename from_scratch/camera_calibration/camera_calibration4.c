#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"
#include "QR_decomposition.h"
#define NUM_POINTS 6


// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float u; float v;
    for (int j =0; j < 6; j++) {
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
    for (int i =0; i < 6; i++) {
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

int main() {
    matrice* variables=matrice_nulle_pointeur(3*6,3*6);
    matrice* zero=matrice_nulle_pointeur(18,1);
    load_points_images("points/points_img5.txt", variables,0);
    load_points_reels("points/points_reel.txt", variables,0);
    /*print_matrice(*variables);*/
    matrice var2=produit(*variables,transposee(*variables));
    matrice valeurs_propres, vecteurs_propres;
    QR_algorithm(*variables,&valeurs_propres, &vecteurs_propres);
    // Affichage des résultats
/*    printf("Valeurs propres :\n");
    print_matrice(valeurs_propres);  
    printf("\nVecteurs propres (colonnes de Q) :\n");
    print_matrice(vecteurs_propres);*/
    matrice P_col=matrice_nulle(18,1);
    P_col.mat[0][0]=vecteurs_propres.mat[0][17] ;P_col.mat[1][0]=vecteurs_propres.mat[1][17] ;P_col.mat[2][0]=vecteurs_propres.mat[2][17] ;P_col.mat[3][0]=vecteurs_propres.mat[3][17] ;
    P_col.mat[4][0]=vecteurs_propres.mat[4][17] ;P_col.mat[5][0]=vecteurs_propres.mat[5][17] ;P_col.mat[6][0]=vecteurs_propres.mat[6][17] ;P_col.mat[7][0]=vecteurs_propres.mat[7][17] ;
    P_col.mat[8][0]=vecteurs_propres.mat[8][17] ;P_col.mat[9][0]=vecteurs_propres.mat[9][17] ;P_col.mat[10][0]=vecteurs_propres.mat[10][17] ;P_col.mat[11][0]=vecteurs_propres.mat[11][17] ;
    P_col.mat[13][0]=0.01 ;P_col.mat[14][0]=0.01 ;P_col.mat[15][0]=0.01 ;P_col.mat[16][0]=0.01 ;P_col.mat[17][0]=0.01 ;
/*    printf("vp: %Lf\n",vecteurs_propres.mat[4][17]);
    printf("P:%Lf\n",P_col.mat[4][0]);*/
    /*print_matrice(P_col);*/
    matrice Test=produit(*variables,P_col);
    /*print_matrice(Test);*/
    matrice P=matrice_nulle(3,4);
    P.mat[0][0]=P_col.mat[0][0] ;P.mat[0][1]=P_col.mat[1][0] ;P.mat[0][2]=P_col.mat[2][0] ;P.mat[0][3]=P_col.mat[3][0] ;
    P.mat[1][0]=P_col.mat[4][0] ;P.mat[1][1]=P_col.mat[5][0] ;P.mat[1][2]=P_col.mat[6][0] ;P.mat[1][3]=P_col.mat[7][0] ;
    P.mat[2][0]=P_col.mat[8][0] ;P.mat[2][1]=P_col.mat[9][0] ;P.mat[2][2]=P_col.mat[10][0] ;P.mat[2][3]=P_col.mat[11][0] ;
    print_matrice(P);
    matrice point_test=matrice_nulle(4,1);
    point_test.mat[3][0]=1;
    print_matrice(produit(P,point_test));
    return 0;
}
