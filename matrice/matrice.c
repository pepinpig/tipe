#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"


matrice matrice_nulle(int n, int m){
    matrice a;
    a.n = n;
    a.m = m;
    a.mat = calloc(a.n,sizeof(int*));
    for (int i = 0; i < a.n; i++) {
        a.mat[i] = calloc(a.m,sizeof(int));
    }
    return a;
}


void print_matrice (matrice a) {
    for(int i=0; i<a.n; i++){
        for(int j=0;j<a.m;j++){
            printf("%d ",a.mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_matrice(matrice a){
    for (int i = 0; i < a.n; i++) {
        free(a.mat[i]);
    }
    free(a.mat);
}

matrice somme(matrice a, matrice b) {
    assert(a.n == b.n);
    assert(a.m == b.m);

    matrice c = matrice_nulle(a.n,a.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            c.mat[i][j] = a.mat[i][j] + b.mat[i][j];
        }
    }

    return c;
}


matrice produit(matrice a, matrice b) {
    assert(a.m == b.n);
    matrice c = matrice_nulle(a.n,b.m);
    for (int i = 0; i < a.n; i++) {
        for (int j=0; j<b.m; j++){
            for (int k=0; k<b.n; k++){
                c.mat[i][j]=c.mat[i][j]+a.mat[i][k]*b.mat[k][j];
            }
        }
    }
    return c;
}

matrice transposee(matrice a) {
    matrice at = matrice_nulle(a.m,a.n);
    for (int i = 0; i < a.n; i++) {
        for (int j=0; j<a.m; j++){
            at.mat[j][i]=a.mat[i][j];
        }
    }
    return at;
}



