#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define EPSILON 1e-9 // Pour vérifier si un nombre est proche de zéro

void printMatrix(double matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%10.5f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void gaussJordan(double matrix[MAX_ROWS][MAX_COLS], int rows, int cols) {
    int i, j, k;
    for (i = 0, j = 0; i < rows && j < cols; j++) {
        // Trouver le pivot maximum dans la colonne courante
        int maxRow = i;
        for (k = i + 1; k < rows; k++) {
            if (fabs(matrix[k][j]) > fabs(matrix[maxRow][j])) {
                maxRow = k;
            }
        }

        // Si le pivot est trop petit, passe à la colonne suivante
        if (fabs(matrix[maxRow][j]) < EPSILON) {
            continue;
        }

        // Échange la ligne courante avec la ligne du pivot
        for (k = j; k < cols; k++) {
            double temp = matrix[i][k];
            matrix[i][k] = matrix[maxRow][k];
            matrix[maxRow][k] = temp;
        }

        // Normaliser la ligne courante
        double pivot = matrix[i][j];
        for (k = j; k < cols; k++) {
            matrix[i][k] /= pivot;
        }

        // Éliminer les autres lignes
        for (k = 0; k < rows; k++) {
            if (k != i && fabs(matrix[k][j]) > EPSILON) {
                double factor = matrix[k][j];
                for (int l = j; l < cols; l++) {
                    matrix[k][l] -= factor * matrix[i][l];
                }
            }
        }

        // Passe à la ligne suivante
        i++;
    }
}

int main() {
    int rows, cols;
    double matrix[MAX_ROWS][MAX_COLS];

    printf("Entrez le nombre de lignes: ");
    scanf("%d", &rows);
    printf("Entrez le nombre de colonnes (avec le vecteur de résultat inclus): ");
    scanf("%d", &cols);

    printf("Entrez les éléments de la matrice augmentée (lignes par colonnes):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%lf", &matrix[i][j]);
        }
    }

    printf("\nMatrice augmentée initiale:\n");
    printMatrix(matrix, rows, cols);

    gaussJordan(matrix, rows, cols);

    printf("\nMatrice augmentée après Gauss-Jordan:\n");
    printMatrix(matrix, rows, cols);

    // Afficher les solutions
    printf("\nSolutions (si unique) ou dernières colonnes réduites:\n");
    for (int i = 0; i < rows; i++) {
        printf("x%d = %10.5f\n", i + 1, matrix[i][cols - 1]);
    }

    return 0;
}
