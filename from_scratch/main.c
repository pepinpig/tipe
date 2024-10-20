#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include "point.h"
#include "e_mat.h"
#define MAX_POINTS 15



int main() {
    matrice test=matrice_nulle(3,1);
    test.mat[0][0]=560; test.mat[1][0]=341; test.mat[2][0]=1;
    matrice Mat=calcul_mat();
    matrice res=produit(Mat,test);
    print_matrice(res);
    multiplication_scalaire(res, 1/(res.mat[2][0]));
    print_matrice(res);
    return 0;
}
