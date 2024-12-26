#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "matrice.h"
#include "SVD.h"
#include "manipulation_fichiers.h"
#define precision 1e-18


// Fonction pour calculer la norme d'un vecteur colonne
long double norme_vecteur_colonne(matrice a, int colonne) {
    long double somme = 0.0;
    for (int i = 0; i < a.n; i++) {
        somme += a.mat[i][colonne] * a.mat[i][colonne];
    }
    return sqrt(somme);
}

// Décomposition QR (Gram-Schmidt)
void decomposition_QR(matrice* A, matrice* Q, matrice* R) {
    int n = A->n, m = A->m;

    // Allocation pour Q et R
    *Q = matrice_nulle(n, n);
    *R = matrice_nulle(n, m);

    // Gram-Schmidt
    for (int j = 0; j < n; j++) {
        // Copier la colonne j de A dans Q
        for (int i = 0; i < n; i++) {
            Q->mat[i][j] = A->mat[i][j];
        }

        // Orthogonalisation
        for (int k = 0; k < j; k++) {
            long double dot_product = 0.0;
            for (int i = 0; i < n; i++) {
                dot_product += Q->mat[i][k] * A->mat[i][j];
            }
            R->mat[k][j] = dot_product;
            for (int i = 0; i < n; i++) {
                Q->mat[i][j] = Q->mat[i][j] - (R->mat[k][j] * Q->mat[i][k]);
            }
        }

        // Normalisation
        R->mat[j][j] = norme_vecteur_colonne(*Q, j);
        for (int i = 0; i < n; i++) {
            if (R->mat[j][j]>precision){
                Q->mat[i][j] /= R->mat[j][j];
            }
        }


    }
}

// Décomposition RQ
void decomposition_RQ(matrice* A, matrice* Q, matrice* R) {
    assert(A->n==A->m);
    int n = A->n;
    matrice P =matrice_nulle(n,n);
    for (int i = 0; i < n; ++i){
        P.mat[i][n-1-i]=1;
    }
    matrice Ap=produit(P,*A);
    assert(matrice_egale(produit(produit(P,P),*A),*A));
    matrice Apt=transposee(Ap);
    matrice Qb,Rb;
    decomposition_QR(&Apt,&Qb,&Rb);
    assert(matrice_egale(produit(Qb,Rb),Apt));
    matrice proQR=produit(Qb,Rb);
    *Q=produit(P,transposee(Qb));
    *R=produit(produit(P,transposee(Rb)),P);
    assert(matrice_egale(produit(P,transposee(produit(Qb,Rb))),*A));
    assert(matrice_egale(produit(*R,*Q),*A));
}
//Algorithm QR
void qr_algorithm(matrice *A, matrice* S) {
    matrice At = transposee(*A);
    matrice AtA = produit(At, *A);
    matrice Q_accum = matrice_identite(AtA.n);
    print_matrice(AtA);
    matrice B = AtA;
    int iters = 0;
    long double diff = 1e9; 
    while ((diff > precision)&&(iters<1e3)&&(iters>20)  ) {
        matrice Q, R;
        /*print_matrice(B);*/
        decomposition_QR(&B, &Q, &R);
        Q_accum = produit(Q_accum, Q);
        matrice B_new = produit(R, Q);
        diff = 0.0;
        for (int i = 0; i < B_new.n; i++) {
            diff += fabs(B_new.mat[i][i] - B.mat[i][i]);
        }
        free_matrice(B);
        B = B_new;
        iters++;
    }
    *S = matrice_nulle(B.n, B.m);
    for (int i = 0; i < B.n; i++) {
        if (B.mat[i][i]>0){
            S->mat[i][i] = sqrt(B.mat[i][i]);
        }
    }
    printf("Convergence atteinte après %d itérations.\n", iters);
    print_matrice(*S);
    free_matrice(At);
    free_matrice(Q_accum);
    free_matrice(B);
}

void eigen_decomposition(matrice *A, matrice* S, matrice* V) {
    matrice At = transposee(*A);
    matrice AtA = produit(At, *A);
    matrice AAt = produit(*A, At);

    // Initialiser les matrices Q et R pour les deux décompositions QR
    matrice Q_accum_AAt = matrice_identite(AAt.n);
    matrice Q_accum_AtA = matrice_identite(AtA.n);

    // Itération QR pour A^T A (calcul de V)
    matrice B_AtA = AtA;
    int iters_AtA = 0;
    // Construire la matrice S (valeurs singulières sur la diagonale)
    *S = matrice_nulle(B_AtA.n, B_AtA.m);
    long double diff_AtA = 1e9; // Grande valeur initiale
    while ((diff_AtA > precision)||(iters_AtA<10)) {
        // Décomposition QR de A^T A
        matrice Q_AtA, R_AtA;
        decomposition_QR(&B_AtA, &Q_AtA, &R_AtA);
        Q_accum_AtA = produit(Q_accum_AtA, Q_AtA); // Accumuler pour obtenir V

        // Calcul de la nouvelle matrice B_AtA
        matrice B_AtA_new = produit(R_AtA, Q_AtA);
        // Convergence : norme de la différence diagonale
        diff_AtA = 0.0;
        for (int i = 0; i < B_AtA_new.n; i++) {
            diff_AtA += fabs(B_AtA_new.mat[i][i] - B_AtA.mat[i][i]);
        }
        // Libération et mise à jour
        free_matrice(B_AtA);
        B_AtA = B_AtA_new;

        iters_AtA++;
    }

    // Construire la matrice S (valeurs singulières sur la diagonale)
    for (int i = 0; i < B_AtA.n; i++) {
        if (B_AtA.mat[i][i]>0){
            S->mat[i][i] = sqrt(B_AtA.mat[i][i]);
        }
    }
    *V = Q_accum_AtA;

    printf("Convergence atteinte après %d iterations pour AtA.\n", iters_AtA);
    print_matrice(*S);
    // Nettoyage
/*    free_matrice(At);
    free_matrice(AtA);
    free_matrice(AAt);
    free_matrice(Q_accum_AAt);
    free_matrice(Q_accum_AtA);
    free_matrice(B_AAt);
    free_matrice(B_AtA);*/
}


void qr_algorithm_SVD(matrice *A, matrice *U, matrice *S, matrice *V) {
    eigen_decomposition(A, S, V); // Eig decomposition

    // Compute U using A and V
    *U = matrice_nulle(A->n, A->n);
    for (int i = 0; i < S->n; i++) {
        long double sigma = S->mat[i][i];
        if (sigma > precision) {
            // Compute u_i = A * v_i / sigma
            matrice v_i = matrice_colonne(*V, i);
            matrice u_i = produit(*A, v_i);
            multiplication_scalaire(u_i, 1/sigma);
            set_colonne(U, i, u_i);
            free_matrice(u_i);
            free_matrice(v_i);
        }
    }
}

long double min_eig(matrice A) {
    matrice S;
    qr_algorithm(&A, &S);
    long double seuil = precision;
    long double min_sigma = 0.0;
    int i = S.n - 1;
    while ((i >= 0)&&fabs(min_sigma<seuil)) {
        if (fabs(S.mat[i][i]) > seuil) {
            min_sigma = S.mat[i][i];
        }
        i--; 
    }
    free_matrice(S);
    return min_sigma;
}



int test(){
    int n = 4, m = 4;
    matrice A = matrice_nulle(n, m);
    // Remplir la matrice A avec des valeurs pour l'exemple
    A.mat[0][0] = -1; A.mat[0][1] = 0; A.mat[0][2] = 0; A.mat[0][3] = 0;
    A.mat[1][0] = 0; A.mat[1][1] = -1; A.mat[1][2] = 0; A.mat[1][3] = 0;
    A.mat[2][0] = -1; A.mat[2][1] = 0; A.mat[2][2] = 0.1; A.mat[2][3] = 0.5;
    A.mat[3][0] = 0; A.mat[3][1] = -1; A.mat[3][2] = 0; A.mat[3][3] = 0;
    printf("Matrice A :\n");
    print_matrice(A);
    matrice* U=matrice_nulle_pointeur(A.n,A.n);
    matrice* S=matrice_nulle_pointeur(A.n,A.m);
    matrice* V=matrice_nulle_pointeur(A.m,A.m);
    printf("test");
    qr_algorithm_SVD(&A, U, S, V);
    printf("U\n");
    print_matrice(*U);
    printf("S\n");
    printf("val sing min : %6.10Lf\n", S->mat[3][3]);
    print_matrice(*S);
    printf("V\n");
    print_matrice(*V);
    return 0;
}
