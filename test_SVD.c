#include "SVD.h"

// Vérifie que A ≈ U * S * V^T
int verifier_reconstruction(matrice* A, matrice* U, matrice* S, matrice* V) {
    matrice* Vt = transposee(V);
    matrice* US = produit(U, S);
    matrice* A_reconstruit = produit(US, Vt);

    printf("\nReconstruction A ≈ U * S * V^T :\n");
    print_matrice(A_reconstruit);

    int ok = matrice_egale(A, A_reconstruit);

    free_matrice(Vt);
    free_matrice(US);
    free_matrice(A_reconstruit);
    return ok;
}


int main() {
    int n = 12, m = 12;
    matrice* A1 = matrice_nulle(n, m);
    double tab[12][12] = {
        {-0.001,  0.002, -0.000,  0.001,  0.214,  0.477,  0.173, -0.450, -0.097, -0.215, -0.652,  0.113},
        {-0.001, -0.000,  0.001,  0.001,  0.220,  0.507,  0.220,  0.375,  0.012, -0.427,  0.444,  0.354},
        {-0.001, -0.001, -0.001,  0.001,  0.256, -0.108,  0.193, -0.039, -0.600,  0.534,  0.080,  0.482},
        {-0.001,  0.000, -0.000, -0.001,  0.379,  0.358,  0.292, -0.026,  0.301,  0.535,  0.176, -0.486},
        {-0.000,  0.001, -0.000,  0.001, -0.277, -0.128,  0.588,  0.525,  0.228,  0.130, -0.444,  0.140},
        {-0.000, -0.000,  0.001,  0.001, -0.273, -0.162,  0.465, -0.615,  0.330, -0.038,  0.350,  0.268},
        {-0.001, -0.000, -0.000,  0.001, -0.431,  0.474, -0.428,  0.013,  0.276,  0.426, -0.032,  0.384},
        {-0.001,  0.000,  0.000,  0.000, -0.600,  0.328,  0.233, -0.028, -0.550, -0.017,  0.143, -0.393},
        { 0.263, -0.870,  0.089, -0.408, -0.001,  0.001,  0.001, -0.000,  0.000, -0.000, -0.002,  0.001},
        { 0.324,  0.167, -0.869, -0.336, -0.001,  0.001,  0.001, -0.000,  0.000, -0.001,  0.001,  0.001},
        { 0.542,  0.454,  0.487, -0.512, -0.000,  0.001,  0.000,  0.000, -0.001,  0.002,  0.000,  0.001},
        { 0.729, -0.099, -0.009,  0.677,  0.001,  0.001,  0.001, -0.000, -0.000,  0.001,  0.001, -0.001}
    };
    

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            A1->mat[i][j] = tab[i][j];

    printf("Matrice A :\n");
    print_matrice(A1);

    matrice* S = matrice_nulle(A1->n, A1->m);
    matrice* V = matrice_nulle(A1->m, A1->m);
    matrice* U = matrice_nulle(A1->n, A1->n);
    qr_algorithm_SVD(A1, U, S, V);

    printf("\nMatrice U :\n");
    print_matrice(U);
    printf("\nMatrice S :\n");
    print_matrice(S);
    printf("\nMatrice V :\n");
    print_matrice(V);

    if (verifier_reconstruction(A1, U, S, V)) {
        printf("\n✅ A ≈ U * S * V^T est vérifié.\n");
    } else {
        printf("\n❌ Erreur de reconstruction A ≈ U * S * V^T.\n");
    }
    if (verifier_orthogonalite(U)) {
        printf("✅ U est orthogonale.\n");
    } else {
        printf("❌ U n'est pas orthogonale.\n");
        printf("UtU:\n");
        print_matrice(produit(transposee(U),U));
    }

    if (verifier_orthogonalite(V)) {
        printf("✅ V est orthogonale.\n");
    } else {
        printf("❌ V n'est pas orthogonale.\n");
        printf("VtV:\n");
        print_matrice(produit(transposee(V),V));
    }

    free_matrice(A1);
    free_matrice(U);
    free_matrice(S);
    free_matrice(V);
    return 0;
}
