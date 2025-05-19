#include "SVD.h"

double norme_vecteur_colonne(matrice a, int colonne) {
    double somme = 0.0;
    for (int i = 0; i < a.n; i++) {
        somme += a.mat[i][colonne] * a.mat[i][colonne];
    }
    return sqrt(somme);
}

void normaliser_colonne(matrice* u) {
    double norme = 0.0;
    for (int i = 0; i < u->n; i++) {
        norme += u->mat[i][0] * u->mat[i][0];
    }
    norme = sqrt(norme);
    if (norme > precision) {
        for (int i = 0; i < u->n; i++) {
            u->mat[i][0] /= norme;
        }
    }
}

int verifier_orthogonalite(matrice* M) {
    matrice* Mt = transposee(M);
    matrice* identite = produit(Mt, M);
    int ok = matrice_egale(identite, matrice_identite(M->m));
    free_matrice(Mt);
    free_matrice(identite);
    return ok;
}

void decomposition_QR(matrice* A, matrice* Q, matrice* R) {
    int n=A->n;
    // Gram-Schmidt
    for (int j = 0; j < n; j++) {
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

    assert(verifier_orthogonalite(Q));
    }
}
void decomposition_QR_householder(matrice* A, matrice* Q, matrice* R) {
    int n = A->n;
    int m = A->m;

    // Initialisation
    matrice* Q_temp = matrice_identite(n);
    matrice* A_copy=matrice_nulle(A->n,A->n);
    copie_matrice(A,A_copy);

    for (int k = 0; k < n && k < m; ++k) {
        // Calcul de la norme de la colonne à partir de l'indice k
        long double norm = 0.0;
        for (int i = k; i < n; ++i) {
            norm += A_copy->mat[i][k] * A_copy->mat[i][k];
        }
        norm = sqrtl(norm);

        if (fabsl(norm) < precision) continue;

        // Création du vecteur de Householder
        long double v[n];
        for (int i = 0; i < n; ++i) v[i] = 0.0;

        for (int i = k; i < n; ++i) {
            v[i] = A_copy->mat[i][k];
        }
        v[k] += (v[k] >= 0 ? norm : -norm); // signe pour éviter annulation numérique

        // Normalisation
        long double v_norm = 0.0;
        for (int i = k; i < n; ++i) {
            v_norm += v[i] * v[i];
        }
        v_norm = sqrtl(v_norm);
        for (int i = k; i < n; ++i) {
            v[i] /= v_norm;
        }

        // Appliquer H = I - 2vv^T à A_copy
        for (int j = k; j < m; ++j) {
            long double dot = 0.0;
            for (int i = k; i < n; ++i) {
                dot += v[i] * A_copy->mat[i][j];
            }
            for (int i = k; i < n; ++i) {
                A_copy->mat[i][j] -= 2 * v[i] * dot;
            }
        }

        // Appliquer H à Q_temp
        for (int j = 0; j < n; ++j) {
            long double dot = 0.0;
            for (int i = k; i < n; ++i) {
                dot += v[i] * Q_temp->mat[i][j];
            }
            for (int i = k; i < n; ++i) {
                Q_temp->mat[i][j] -= 2 * v[i] * dot;
            }
        }
    }
    assert(verifier_orthogonalite(Q_temp));

    // Transposer Q_temp pour obtenir Q
    matrice* Q_final = transposee(Q_temp);
    copie_matrice(Q_final, Q);
    copie_matrice(A_copy, R);

    // Libérations
    free_matrice(Q_temp);
    free_matrice(Q_final);
    free_matrice(A_copy);
}


void qr_algorithm(matrice *A, matrice* S) {
    matrice* At = transposee(A);
    matrice* AtA = produit(At, A);
    matrice* Q_accum = matrice_identite(AtA->n);
 /*   print_matrice(AtA);*/
    matrice* B = AtA;
    int iters = 0;
    long double diff = 1e9; 
    while ((diff > precision)&&(iters<1e3)) {
        matrice *Q, *R;
        /*print_matrice(B);*/
        decomposition_QR_householder(B, Q, R);
        Q_accum = produit(Q_accum, Q);
        matrice* B_new = produit(R, Q);
        diff = 0.0;
        for (int i = 0; i < B_new->n; i++) {
            diff += fabs(B_new->mat[i][i] - B->mat[i][i]);
        }
        free_matrice(B);
        B = B_new;
        iters++;
    }
    S = matrice_nulle(B->n, B->m);
    for (int i = 0; i < B->n; i++) {
        if (B->mat[i][i] > 0){ 
            S->mat[i][i] = sqrt(B->mat[i][i]);
        }
    }
    printf("Convergence atteinte après %d itérations.\n", iters);
    free_matrice(At);
    free_matrice(Q_accum);
    free_matrice(B);
}

void eigen_decomposition(matrice* AtA, matrice* S, matrice* V) {
    int n = AtA->n;
    // B = copie de AtA pour itérer
    matrice* B = matrice_nulle(n, n);
    copie_matrice(AtA, B);
    // Q_accum contient les vecteurs propres (colonnes)
    matrice* Q_accum = matrice_identite(n);

    double epsilon = 1e-12;
    double diff = 1.0;
    int max_iter = 1000;
    int iters = 0;

    while (diff > 1e-9 && iters < max_iter) {
        matrice* Q = matrice_nulle(n, n);
        matrice* R = matrice_nulle(n, n);
        decomposition_QR_householder(B, Q, R);

        matrice* B_new = produit(R, Q);
        matrice* Q_accum_new = produit(Q_accum, Q);

        diff = 0.0;
        for (int i = 0; i < n; i++) {
            diff += fabs(B_new->mat[i][i] - B->mat[i][i]);
        }

        free_matrice(B);
        free_matrice(Q);
        free_matrice(R);
        free_matrice(Q_accum);

        B = B_new;
        Q_accum = Q_accum_new;
        iters++;
    }

    // Stocke les valeurs propres (pas encore racine)
    for (int i = 0; i < n && i < S->m && i < S->n; i++) {
        double lambda = B->mat[i][i];
        S->mat[i][i] = (lambda > epsilon) ? lambda : 0.0;
    }

    // Stocke les vecteurs propres (colonnes de V)
    copie_matrice(Q_accum, V);

    free_matrice(B);
    free_matrice(Q_accum);
}

void qr_algorithm_SVD(matrice* A, matrice* U, matrice* S, matrice* V) {
    int m = A->m;
    int n = A->n;

    // Étape 1 : AtA = A^T * A
    matrice* At = transposee(A);
    matrice* AtA = produit(At, A);

    // Étape 2 : décomposition de AtA → valeurs propres dans S, vecteurs propres dans V
    
    eigen_decomposition(AtA, S, V);  // S contiendra σ² ici
    // Étape 3 : construire U et racine des valeurs propres
    for (int i = 0; i < n; i++) {
        double sigma2 = S->mat[i][i];
        if (sigma2 < 1e-12) continue;

        double sigma = sqrt(sigma2);
        S->mat[i][i] = sigma;  // mettre à jour avec σ

        matrice* v_i = matrice_colonne(V, i);         // vecteur v_i
        matrice* u_i = produit(A, v_i); // u_i = A v_i
        multiplication_scalaire(u_i, 1.0 / sigma);    // u_i /= σ
        normaliser_colonne(u_i);                      // normaliser

        set_colonne(U, i, u_i);

        free_matrice(v_i);
        free_matrice(u_i);
    }

    free_matrice(At);
    free_matrice(AtA);
}

double min_eig(matrice* A) {
    matrice* S;
    qr_algorithm(A, S);
    long double seuil = precision;
    long double min_sigma = 0.0;
    int i = S->n - 1;
    while ((i >= 0)&&fabs(min_sigma<seuil)) {
        if (fabs(S->mat[i][i]) > seuil) {
            min_sigma = S->mat[i][i];
        }
        i--; 
    }
    free_matrice(S);
    return min_sigma;
}

