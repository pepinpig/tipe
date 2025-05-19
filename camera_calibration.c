#include "camera_calibration.h"

matrice* construction_A(double* X, double* Y, double* Z, double* u, double* v, int n) {
    matrice* A = matrice_nulle(2 * n, 12); // 12 colonnes pour les 12 paramètres de P

    for (int i = 0; i < n; i++) {
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
}

void camera_calibration_resolution(matrice* P, matrice* A, matrice* K, matrice* R, matrice* T) {
    // Étape 1 : Décomposition SVD de A
    matrice* S = matrice_nulle(A->n, A->m);
    matrice* V = matrice_nulle(A->m, A->m);
    matrice* U = matrice_nulle(A->n, A->n);
    qr_algorithm_SVD(A, U, S, V);

    // Étape 2 : Extraire le vecteur p (solution homogène de Ap=0)
    int index_min = S->n - 1;
    while (S->mat[index_min][index_min] < EPSILON) {
        index_min--;
        assert(index_min >= 0);
    }
    matrice *p = matrice_nulle(V->m, 1);
    for (int i = 0; i < V->m; i++) {
        p->mat[i][0] = V->mat[i][index_min];
    }

    // Étape 3 : Construire la matrice de projection P (3x4)
    for (int i = 0; i < 12; i++) {
        P->mat[i / 4][i % 4] = p->mat[i][0];
    }

    free_matrice(U);
    free_matrice(S);
    free_matrice(V);
    free_matrice(p);

    // Étape 4 : Extraire le noyau droit de P pour obtenir le centre C*
    matrice* U2 = matrice_nulle(P->n, P->n);
    matrice* S2 = matrice_nulle(P->n, P->m);
    matrice* V2 = matrice_nulle(P->m, P->m);
    qr_algorithm_SVD(P, U2, S2, V2);
    index_min = fmin(P->n - 1, P->m - 1);
    matrice *C_star = matrice_nulle(V2->m, 1);
    for (int i = 0; i < V2->m; i++) {
        C_star->mat[i][0] = V2->mat[i][index_min];
    }
    // Homogénéisation
    double scale = C_star->mat[C_star->n - 1][0];
    for (int i = 0; i < C_star->n; i++) {
        C_star->mat[i][0] /= scale;
    }
    // Étape 5 : QR de M = P[0:3, 0:3]⁻¹
    matrice* M = matrice_nulle(3, 3);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            M->mat[i][j] = P->mat[i][j];

    matrice* M_inv = inverser_matrice(M);
    matrice* Q = matrice_nulle(3, 3);
    matrice* R_temp = matrice_nulle(3, 3);
    decomposition_QR_householder(M_inv, Q, R_temp);
    copie_matrice(R_temp, R);
    // Étape 6 : Extraire R et K
    copie_matrice(R_temp, R); // R temporaire devient R
    matrice* R_inv = inverser_matrice(R);
    matrice* K_temp = produit(R_inv, M); // K = R⁻¹ · M
    multiplication_scalaire(K_temp, 1.0 / K_temp->mat[2][2]); // normalisation
    copie_matrice(K_temp, K);

    // Étape 7 : Calcul de T = -R · C*
    matrice* C_euclidienne = matrice_nulle(3, 1);
    for (int i = 0; i < 3; ++i) {
        C_euclidienne->mat[i][0] = C_star->mat[i][0];
    }
    matrice* RC = produit(R, C_euclidienne);
    multiplication_scalaire(RC, -1.0);
    for (int i = 0; i < 3; ++i)
        T->mat[i][0] = RC->mat[i][0];

    free_matrice(U2);
    free_matrice(S2);
    free_matrice(V2);
    free_matrice(C_star);
    free_matrice(M);
    free_matrice(M_inv);
    free_matrice(Q);
    free_matrice(R_temp);
    free_matrice(R_inv);
    free_matrice(K_temp);
    free_matrice(RC);
}


matrice* compute_F(matrice* K1 ,matrice* R1, matrice* T1, matrice* K2, matrice* R2, matrice* T2){
    //F=(K2​t2​−K2​R2​(K1​R1​)−1K1​t1​)∧(K2​R2​(K1​R1​)−1)
    matrice* KR1 = produit(K1, R1);
    matrice* KR1_inv = inverser_matrice(KR1);
    matrice* temp = produit(K2, R2);      
    matrice* M = produit(temp, KR1_inv);  
    free_matrice(temp);
    matrice* K1T1 = produit(K1, T1);
    matrice* inter = produit(KR1_inv, K1T1);
    matrice* R2_K1T1 = produit(R2, inter);
    matrice* neg_term = produit(K2, R2_K1T1);
    multiplication_scalaire(neg_term, -1); 
    matrice* K2T2 = produit(K2, T2);
    matrice* v = somme(neg_term, K2T2);
    return produit_vectoriel(v, M);
}