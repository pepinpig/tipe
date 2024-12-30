#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include "point.h"
#include "e_mat.h"

int lirePoints(const char *nomFichier, Point** points, int n, int camera) {
    FILE *fichier = fopen(nomFichier, "r");
    int ni;
    fscanf(fichier, "%d", &ni);
    assert(n==ni);
    for (int i = 0; i < n; i++) {
        if (camera == 1) {
            fscanf(fichier, "%Lf %Lf", &points[i]->cam1.mat[0][0], &points[i]->cam1.mat[1][0]);
            fscanf(fichier, "%Lf %Lf", &points[i]->reel.mat[0][0], &points[i]->reel.mat[1][0]);
        } else if (camera == 2) {
            fscanf(fichier, "%Lf %Lf", &points[i]->cam2.mat[0][0], &points[i]->cam2.mat[1][0]);
        }
    }
    fclose(fichier);
    return 0;
}

matrice calcul_mat() {
    int n;
    FILE *fichier1 = fopen("points_img1.txt", "r");
    fscanf(fichier1, "%d", &n);
    fclose(fichier1);

    Point* points[n];
    for (int i = 0; i < n; ++i){
        points[i]=point_vide_pointeur();
    }
    lirePoints("points_img1.txt", points, n,1);
    lirePoints("points_img2.txt", points, n,2);
    matrice C1 = concatenation3(vect3_of_vect2(points[0]->cam1),vect3_of_vect2(points[1]->cam1),vect3_of_vect2(points[2]->cam1));
    matrice C2 = concatenation3(vect3_of_vect2(points[0]->cam2),vect3_of_vect2(points[1]->cam2),vect3_of_vect2(points[2]->cam2));
    matrice C3_1 = vect3_of_vect2(points[3]->cam1);
    matrice C3_2 = vect3_of_vect2(points[3]->cam2);
    matrice* k1=resolution_systeme(&C1,&C3_1);
    matrice* k2=resolution_systeme(&C2,&C3_2);
    float r1=(k1->mat[0][0])/(k2->mat[0][0]);
    float r2=(k1->mat[1][0])/(k2->mat[1][0]);
    float r3=(k1->mat[2][0])/(k2->mat[2][0]);
    matrice C1_t=transposee(C1);
    matrice* Rx=matrice_nulle_pointeur(3,1);
    Rx->mat[0][0]=r1*C2.mat[0][0];
    Rx->mat[1][0]=r2*C2.mat[0][1];
    Rx->mat[2][0]=r3*C2.mat[0][2];
    matrice* Ry=matrice_nulle_pointeur(3,1);
    Ry->mat[0][0]=r1*C2.mat[1][0];
    Ry->mat[1][0]=r2*C2.mat[1][1];
    Ry->mat[2][0]=r3*C2.mat[1][2];
    matrice* R=matrice_nulle_pointeur(3,1);
    R->mat[0][0]=r1;
    R->mat[1][0]=r2;
    R->mat[2][0]=r3;
    matrice* Mat_c1_t=resolution_systeme(&C1_t,Rx);
    matrice* Mat_c2_t=resolution_systeme(&C1_t,Ry);
    matrice* Mat_c3_t=resolution_systeme(&C1_t,R);
    matrice Mat=transposee(concatenation3(*Mat_c1_t,*Mat_c2_t,*Mat_c3_t));
    return Mat;
}