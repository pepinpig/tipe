#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "matrice.h"
#include "SVD.h"
#include "manipulation_fichiers.h"

int main(){
    matrice A = lire_matrice("A-cube.txt");
    printf("Matrice A :\n");
    print_matrice(A);
    matrice S,U,V;
    qr_algorithm_SVD(&A, &U,&S,&V);
    printf("Matrice U :");
    print_matrice(U);
    printf("Matrice S :");
    print_matrice(S);
    printf("Matrice Vt :");
    matrice Vt=transposee(V);
    print_matrice(Vt);
    printf("Matrice reconstruite A :");
    print_matrice(produit(produit(U,S),Vt));
}