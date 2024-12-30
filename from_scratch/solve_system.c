#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_linalg.h>

typedef struct {
    int n;
    int m;
    float** mat;
} matrice;

matrice matrice_nulle(int n, int m);
void print_matrice(matrice a);
void free_matrice(matrice a);
matrice null_space(gsl_vector* S, gsl_matrix* U, int m, int n, long double tolerance);
void solve_homogeneous_system(matrice A);

int main() {
    // Create your matrix A here (homogeneous system Ax = 0)
    matrice A = matrice_nulle(3, 3);
    A.mat[0][0] = 1; A.mat[0][1] = 2; A.mat[0][2] = 3;
    A.mat[1][0] = 4; A.mat[1][1] = 5; A.mat[1][2] = 6;
    A.mat[2][0] = 7; A.mat[2][1] = 8; A.mat[2][2] = 9;

    solve_homogeneous_system(A);

    free_matrice(A);
    return 0;
}

void solve_homogeneous_system(matrice A) {
    int m = A.n; // Number of rows
    int n = A.m; // Number of columns

    // Allocate GSL matrices and vectors
    gsl_matrix* gsl_A = gsl_matrix_alloc(m, n);
    gsl_matrix* U = gsl_matrix_alloc(m, m);
    gsl_matrix* V = gsl_matrix_alloc(n, n);
    gsl_vector* S = gsl_vector_alloc(n);
    gsl_vector* work = gsl_vector_alloc(n);

    // Copy A into GSL matrix
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            gsl_matrix_set(gsl_A, i, j, A.mat[i][j]);

    // Perform SVD
    gsl_linalg_SV_decomp(gsl_A, V, S, work);

    // Get the null space
    matrice null_space_vec = null_space(S, U, m, n, 1e-5); // Tolerance for singular values
    print_matrice(null_space_vec);

    // Free allocated matrices and vectors
    gsl_matrix_free(gsl_A);
    gsl_matrix_free(U);
    gsl_matrix_free(V);
    gsl_vector_free(S);
    gsl_vector_free(work);
    free_matrice(null_space_vec);
}

// Function to find the null space from singular values
matrice null_space(gsl_vector* S, gsl_matrix* U, int m, int n, long double tolerance) {
    matrice result = matrice_nulle(n, 1); // Assuming at most n solutions

    for (int i = 0; i < n; i++) {
        if (gsl_vector_get(S, i) < tolerance) { // Check for small singular value
            for (int j = 0; j < m; j++) {
                result.mat[j][0] = gsl_matrix_get(U, j, i); // Store the vector in the result
            }
            break; // Return the first null space vector found
        }
    }

    return result;
}

matrice matrice_nulle(int n, int m) {
    matrice result;
    result.n = n;
    result.m = m;
    result.mat = malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) {
        result.mat[i] = calloc(m, sizeof(float));
    }
    return result;
}

void free_matrice(matrice a) {
    for (int i = 0; i < a.n; i++) {
        free(a.mat[i]);
    }
    free(a.mat);
}

void print_matrice(matrice a) {
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            printf("%Lf ", a.mat[i][j]);
        }
        printf("\n");
    }
}
