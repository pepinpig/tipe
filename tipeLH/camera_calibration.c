#include "camera_calibration.h"

matrice* contruction_A(long double* X, long double* Y, long double* Z, long double* u, long double* v) {
    matrice* A = matrice_nulle_pointeur(2 * N, 12); // 12 inconnues (p_11 à p_34)

    for (int i = 0; i < N; i++) {
        int row1 = 2 * i;
        int row2 = 2 * i + 1;
        // lambda_i * u_i = ...
        A->mat[row1][0] = X[i];
        A->mat[row1][1] = Y[i];
        A->mat[row1][2] = Z[i];
        A->mat[row1][3] = 1;
        A->mat[row1][8] = -X[i]* u[i];
        A->mat[row1][9] = -Y[i]* u[i];
        A->mat[row1][10] = -Z[i]* u[i];
        A->mat[row1][11] = -u[i];

        // lambda_i * v_i = ...
        A->mat[row2][4] = X[i];
        A->mat[row2][5] = Y[i];
        A->mat[row2][6] = Z[i];
        A->mat[row2][7] = 1;
        A->mat[row2][8] = -X[i]* v[i];
        A->mat[row2][9] = -Y[i]* v[i];
        A->mat[row2][10] = -Z[i]* v[i];
        A->mat[row2][11] = -v[i];

    }
    return A;
    // Résoudre le système linéaire A * P = B pour obtenir les paramètres
    // Libération de la mémoire allouée pour les matrices A et B
/*    free_matrice(A);
    free_matrice(B);*/
}

void camera_calibration_resolution(matrice* P, matrice* A, matrice* K, matrice* R, matrice* T) {
    // Étape 1 : Calcul de la décomposition SVD de A
    matrice U, S, V; // Matrices pour la décomposition SVD
    qr_algorithm_SVD(A, &U, &S, &V);

    // Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
    int index_min = S.n - 1;
/*    for (int i = 0; i < S.n && i < S.m; i++) {
        if (S.mat[i][i] < min_sigma) {
            min_sigma = S.mat[i][i];
            index_min = i;
        }
    }*/
    matrice p = matrice_nulle(V.m, 1);
    for (int i = 0; i < V.m; i++) {
        p.mat[i][0] = V.mat[i][index_min];
    }

    // Libérer U, S 
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);
    
    // Étape 3 : Construire la matrice P

    for (int i = 0; i < 4; i++) { // Ligne 1
        P->mat[0][i] = p.mat[i][0];
    }
    for (int i = 4; i < 8; i++) { // Ligne 2
        P->mat[1][i-4] = p.mat[i][0];
    }
    for (int i = 8; i < 12; i++) { // Ligne 3
        P->mat[2][i-8] = p.mat[i][0];
    }
    // Libération du vecteur p
    free_matrice(p);

    // Étape 4 : Calcul de la décomposition SVD de P
    matrice U2, S2, V2;
    qr_algorithm_SVD(P, &U2, &S2, &V2);

    // Extraction du vecteur singulier droit associé à la plus petite valeur singulière
    index_min = fmin(P->n - 1,P->m-1);

    matrice C_approx = matrice_nulle(V2.m, 1);
    for (int i = 0; i < V2.m; i++) {
        C_approx.mat[i][0] = V2.mat[i][index_min];
    }

    // Normalisation de C pour que le dernier élément soit 1
    long double scale = C_approx.mat[C_approx.n - 1][0];
    for (int i = 0; i < C_approx.n; i++) {
        C_approx.mat[i][0] /= scale;
    }

   // Étape 5 : QR factorisation de P(1:3, 1:3)^-1
    matrice* P_extract=matrice_nulle_pointeur(3,3);
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 3; ++j){
            P_extract->mat[i][j]=P->mat[i][j];
        }
    }
    matrice* P_inv = inverser_matrice(P_extract);
    matrice Q;
    decomposition_QR(P_inv, &Q, R);

    // Étape 6 : Calcul de K, R et T
    K = inverser_matrice(R);
    matrice* R_inv = inverser_matrice(&Q);
    multiplication_scalaire(*K, 1.0 / K->mat[2][2]); // Normalisation de K(3,3) = 1

    *T = produit(*K, *R_inv);
    multiplication_scalaire(*T, -1); // T = -RC

    // Libération de la mémoire
/*    free_matrice(P_inv);
    free_matrice(Q);
    free_matrice(R);
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

matrice* compute_F(matrice* K1,matrice* K2 , matrice* R, matrice* T){
    matrice* res=matrice_nulle_pointeur(3,3);
    res=inverser_matrice(K1);
    *res=transposee(*res);
    matrice* E=compute_E(R,T);
    *res=produit(produit(*res,*E),*(inverser_matrice(K2)));
    return res;
}
