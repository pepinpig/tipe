#include "camera_calibration.h"

matrice* construction_A(double* X, double* Y, double* Z, double* u, double* v) {
    matrice* A = matrice_nulle(2 * N, 12); // 12 colonnes pour les 12 paramètres de P

    for (int i = 0; i < N; i++) {
        int row1 = 2 * i;
        int row2 = 2 * i + 1;

        double Xi = X[i], Yi = Y[i], Zi = Z[i];
        double ui = u[i], vi = v[i];

        // Ligne correspondant à ui
        A->mat[row1][0] = Xi;
        A->mat[row1][1] = Yi;
        A->mat[row1][2] = Zi;
        A->mat[row1][3] = 1.0;
        A->mat[row1][8] = -ui * Xi;
        A->mat[row1][9] = -ui * Yi;
        A->mat[row1][10] = -ui * Zi;
        A->mat[row1][11] = -ui;

        // Ligne correspondant à vi
        A->mat[row2][4] = Xi;
        A->mat[row2][5] = Yi;
        A->mat[row2][6] = Zi;
        A->mat[row2][7] = 1.0;
        A->mat[row2][8] = -vi * Xi;
        A->mat[row2][9] = -vi * Yi;
        A->mat[row2][10] = -vi * Zi;
        A->mat[row2][11] = -vi;
    }

    return A;
}


void camera_calibration_resolution(matrice* P, matrice* A, matrice* K, matrice* R, matrice* T) {
    // Étape 1 : Calcul de la décomposition SVD de A
    matrice* S = matrice_nulle(A->n, A->m);
    matrice* V = matrice_nulle(A->m, A->m);
    matrice* U = matrice_nulle(A->n, A->n);
    qr_algorithm_SVD(A, U, S, V);

    // Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
    int index_min = S->n - 1;
    while (S->mat[index_min][index_min]<EPSILON) {
        index_min --;
        assert(index_min>=0);
    }
    matrice *p = matrice_nulle(V->m, 1);
    for (int i = 0; i < V->m; i++) {
        p->mat[i][0] = V->mat[i][index_min];
    }

    // Libérer U, S 
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);
    // Étape 3 : Construire la matrice P

    for (int i = 0; i < 4; i++) { // Ligne 1
        P->mat[0][i] = p->mat[i][0];
    }
    for (int i = 4; i < 8; i++) { // Ligne 2
        P->mat[1][i-4] = p->mat[i][0];
    }
    for (int i = 8; i < 12; i++) { // Ligne 3
        P->mat[2][i-8] = p->mat[i][0];
    }
    // Libération du vecteur p
    free_matrice(p);
    printf("test2\n");
    // Étape 4 : Calcul de la décomposition SVD de P
    matrice* S2 = matrice_nulle(P->n, P->m);
    matrice* V2 = matrice_nulle(P->m, P->m);
    matrice* U2 = matrice_nulle(P->n, P->n);
    qr_algorithm_SVD(P, U2, S2, V2);

    // Extraction du vecteur singulier droit associé à la plus petite valeur singulière
    index_min = fmin(P->n - 1,P->m-1);
    printf("test2\n");
    matrice *C_approx = matrice_nulle(V2->m, 1);
    for (int i = 0; i < V2->m; i++) {
        C_approx->mat[i][0] = V2->mat[i][index_min];
    }
    printf("test2\n");
    // Normalisation de C pour que le dernier élément soit 1
    double scale = C_approx->mat[C_approx->n - 1][0];
    for (int i = 0; i < C_approx->n; i++) {
        C_approx->mat[i][0] /= scale;
    }
    printf("test6\n");
   // Étape 5 : QR factorisation de P(1:3, 1:3)^-1
    matrice* P_extract=matrice_nulle(3,3);
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            P_extract->mat[i][j]=P->mat[i][j];
        }
    }
    matrice* P_inv = inverser_matrice(P_extract);
    matrice *Q = matrice_nulle(P_inv->n,P_inv->n);
    printf("test7\n");
    decomposition_QR_householder(P_inv, Q, R);
    printf("test8\n");
    // Étape 6 : Calcul de K, R et T
    K = inverser_matrice(R);
    matrice* R_inv = inverser_matrice(Q);
    multiplication_scalaire(K, 1.0 / K->mat[2][2]); // Normalisation de K(3,3) = 1

    T = produit(K, R_inv);
    multiplication_scalaire(T, -1); // T = -RC

    // Libération de la mémoire
/*    free_matrice(P_inv);
    free_matrice(Q);
    free_matrice(R)
    free_matrice(K);
    free_matrice(R_inv);
    free_matrice(T);
    free_matrice(C_approx);
    free_matrice(U);
    free_matrice(S2);
    free_matrice(V2);*/
}


matrice* compute_E(matrice* R, matrice* T){
    return produit_vectoriel(T,R);
}

