#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "matrice.h"
#include "SVD.h"

void nom_fichier(char* filename, char* matrix_name, char* image_name) {
    snprintf(filename, 256, "%s-%s.txt", matrix_name, image_name);
}

void read_matrice_from_file_dimension(matrice* mtx, char* filename) {
    FILE* fichier = fopen(filename, "r");
    assert(fichier != NULL);
    int n, m;
    fscanf(fichier, "%d %d", &n, &m); 

    mtx = matrice_nulle_pointeur(n, m); 

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(fichier, "%Lf", &mtx->mat[i][j]);
        }
    }

    fclose(fichier);
    printf("Matrice lues depuis %s\n", filename);
}


void save_matrice_to_file_dimension(matrice* matrix, char* filename) {
    FILE* file = fopen(filename, "w");
    assert(file != NULL);

    fprintf(file, "%d %d", matrix->n, matrix->m);
    fprintf(file, "\n");
    for (int i = 0; i < matrix->n; ++i) {
        for (int j = 0; j < matrix->m; ++j) {
            fprintf(file, "%Lf ", matrix->mat[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Matrice enregistrée dans %s\n", filename);
}


void save_matrice_to_file(matrice *A, char* filename) {
    FILE *file = fopen(filename, "w");
    assert(file != NULL);

    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            fprintf(file, "%Lf ", A->mat[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Matrice enregistrée dans %s\n", filename);
}

void read_matrice_from_file(matrice* A, const char *filename) {
    FILE *file = fopen(filename, "r");
    assert(file != NULL);

    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            fscanf(file, "%Lf", &A->mat[i][j]);
        }
    }
    fclose(file);
    printf("Matrice lues depuis %s\n", filename);
}


void load_all_points_images(const char* filename, long double* u, long double*v) {
    FILE *file = fopen(filename, "r");
    long double ui, vi;
    for (int i = 0; i < 6; i++) {
        fscanf(file, "%Lf %Lf", &ui, &vi);
        u[i]=ui;
        v[i]=vi;
    }
    fclose(file);
}


void load_all_points_reels(const char* filename, long double* X, long double* Y, long double* Z) {
    FILE *file = fopen(filename, "r");
    long double Xi, Yi, Zi;
    for (int i = 0; i < 6; i++) {
        fscanf(file, "%Lf %Lf %Lf", &Xi, &Yi, &Zi);
        X[i]=Xi;
        Y[i]=Yi;
        Z[i]=Zi;
    }
    fclose(file);
}