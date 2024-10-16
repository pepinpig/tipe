#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include "point.h"
#define MAX_POINTS 15

int test_matrice() {
    matrice a= matrice_nulle(2,3);
    matrice b= matrice_nulle(3,3);

    a.mat[0][0] = 1; a.mat[0][1] = 3; a.mat[0][2] = 0;
    a.mat[1][0] = 1; a.mat[1][1] = 2; a.mat[1][2] = 1;
    
    b.mat[0][0] = 2; b.mat[0][1] = 0; b.mat[0][2] = 0;
    b.mat[1][0] = 0; b.mat[1][1] = 1; b.mat[1][2] = 1;
    b.mat[2][0] = 1; b.mat[2][1] = 0; b.mat[2][2] = 1;
    
    print_matrice(a);
    print_matrice(b);

/*    matrice c=somme(a,b);
    print_matrice(c);
    free_matrice(c);*/

    matrice d=produit(a,b);
    print_matrice(d);
    free_matrice(d);

    matrice t=transposee(a);
    print_matrice(t);
    free_matrice(a);
    free_matrice(b);
    return 0;
}

/*int test_lecture_point() {
        for (int i = 0; i < n1; i++) {
        printf("Point %d : Caméra 1 -> (%0.1f, %0.1f), Caméra 2 -> (%0.1f, %0.1f)\n",
               i + 1, points[i].cam1.x, points[i].cam1.y,
               points[i].cam2.x, points[i].cam2.y);
    }
    return 0;
}*/
// Exemple d'application de Gauss-Jordan sur la matrice M donnée
int test_Gauss_Jordan() {
    matrice M = matrice_nulle(4, 4);
    
    M.mat[0][0] = 2;  M.mat[0][1] = 4;  M.mat[0][2] = -8;  M.mat[0][3] = 10;
    M.mat[1][0] = -2; M.mat[1][1] = -4; M.mat[1][2] = 8;   M.mat[1][3] = 10;
    M.mat[2][0] = 3;  M.mat[2][1] = 9;  M.mat[2][2] = -3;  M.mat[2][3] = 6;
    M.mat[3][0] = 1;  M.mat[3][1] = 4;  M.mat[3][2] = 2;   M.mat[3][3] = -2;

    printf("Matrice initiale :\n");
    print_matrice(M);   
    Gauss_Jordan(&M);
    print_matrice(M);   
    return 0;
}

int test_inverser() {
    // Exemple de matrice 3x3
    matrice* M = matrice_nulle_pointeur(3, 3);
    M->mat[0][0] = 1;
    M->mat[0][1] = 0;
    M->mat[0][2] = 5;
    M->mat[1][0] = 2;
    M->mat[1][1] = 1;
    M->mat[1][2] = 6;
    M->mat[2][0] = 3;
    M->mat[2][1] = 4;
    M->mat[2][2] = 0;
    
    printf("Matrice M:\n");
    print_matrice(*M);
    
    // Inversion de la matrice
    matrice* inv = inverser_matrice(M);
    
    printf("\nMatrice inverse de M:\n");
    print_matrice(*inv);
    
    return 0;
}
int test_resolution_system() {
    // Définir la matrice A (3x3) comme pointeur
    matrice* A = matrice_nulle_pointeur(3, 3);
    A->mat[0][0] = 1; A->mat[0][1] = 2; A->mat[0][2] = -1;
    A->mat[1][0] = 1; A->mat[1][1] = 1; A->mat[1][2] = -2;
    A->mat[2][0] = 2; A->mat[2][1] = 1; A->mat[2][2] = -1;

    // Définir le vecteur V (3x1) comme pointeur
    matrice* V = matrice_nulle_pointeur(3, 1);
    V->mat[0][0] = 2;
    V->mat[1][0] = -3;
    V->mat[2][0] = 1;

    // Résoudre le système AU = V
    printf("resolution du systeme");
    matrice* U = resolution_systeme(A, V);

    // Afficher la solution U
    print_matrice(*U);  // Déréférencer U pour l'affichage

    // Libérer la mémoire allouée pour U si nécessaire
    // free(U->mat);
    // free(U);

    return 0;
}

int lirePoints(const char *nomFichier, Point** points, int n, int camera) {
    FILE *fichier = fopen(nomFichier, "r");
    int ni;
    fscanf(fichier, "%d", &ni);
    assert(n==ni);
    for (int i = 0; i < n; i++) {
        if (camera == 1) {
            fscanf(fichier, "%f %f", &points[i]->cam1.mat[0][0], &points[i]->cam1.mat[1][0]);
            fscanf(fichier, "%f %f", &points[i]->reel.mat[0][0], &points[i]->reel.mat[1][0]);
        } else if (camera == 2) {
            fscanf(fichier, "%f %f", &points[i]->cam2.mat[0][0], &points[i]->cam2.mat[1][0]);
        }
    }
    fclose(fichier);
    return 0;
}

int main() {
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
    print_point(points[0]);
    matrice C1 = concatenation3(vect3_of_vect2(points[0]->cam1),vect3_of_vect2(points[1]->cam1),vect3_of_vect2(points[2]->cam1));
    matrice C2 = concatenation3(vect3_of_vect2(points[0]->cam2),vect3_of_vect2(points[1]->cam2),vect3_of_vect2(points[2]->cam2));
    print_matrice(C1);
    print_matrice(C2);
    return 0;
}
