#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"


int main() {
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