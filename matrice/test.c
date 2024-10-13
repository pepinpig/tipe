#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Structure de matrice
typedef struct {
    int n;    // nombre de lignes
    int m;    // nombre de colonnes
    float **mat;  // tableau 2D de float
} matrice;

// Fonction d'échange de lignes
void echange_ligne(matrice *a, int i, int j) {
    for (int k = 0; k < a->m; k++) {
        float temp = a->mat[i][k];
        a->mat[i][k] = a->mat[j][k];
        a->mat[j][k] = temp;
    }
}

// Multiplie la ligne i par lambda
void multiplication_ligne(matrice *a, int i, float lambda) {
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] *= lambda;
    }
}

// Ajoute lambda fois la ligne j à la ligne i
void ajout_ligne(matrice *a, int i, int j, float lambda) {
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] += lambda * a->mat[j][k];
    }
}

// Recherche du pivot partiel
int choix_pivot(matrice *a, int i) {
    int n = a->n;
    int max_index = i;
    float max_value = fabs(a->mat[i][i]);
    
    for (int k = i + 1; k < n; k++) {
        if (fabs(a->mat[k][i]) > max_value) {
            max_value = fabs(a->mat[k][i]);
            max_index = k;
        }
    }
    
    if (max_value == 0) {
        return -1;  // Retourne -1 si aucun pivot n'est trouvé
    }
    
    return max_index;
}

// Affichage de la matrice pour voir les transformations
void afficher_matrice(matrice *a) {
    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < a->m; j++) {
            printf("%6.2f ", a->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Algorithme de Gauss-Jordan
void Gauss_Jordan(matrice *a) {
    int n = a->n;

    for (int i = 0; i < n; ++i) {
        printf("Étape %d :\n", i+1);
        afficher_matrice(a);
        int pivot = choix_pivot(a, i);
        if (pivot == -1) {
            printf("Pas de pivot trouvé à la colonne %d. La matrice peut être singulière.\n", i);
            continue;  // Passe à la colonne suivante
        }
        echange_ligne(a, i, pivot);
        float pivot_value = a->mat[i][i];
        if (pivot_value != 0) {
            multiplication_ligne(a, i, 1.0 / pivot_value);
        }
        printf("Après unitarisation de la ligne %d :\n", i);
        afficher_matrice(a);

        // Élimination dans les autres lignes
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                float lambda = -a->mat[k][i];
                ajout_ligne(a, k, i, lambda);
            }
        }

        printf("Après élimination des autres lignes :\n");
        afficher_matrice(a);
    }
}

// Fonction pour créer une matrice dynamique
matrice *creer_matrice(int n, int m) {
    matrice *mat = malloc(sizeof(matrice));
    mat->n = n;
    mat->m = m;
    mat->mat = malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        mat->mat[i] = malloc(m * sizeof(float));
    }
    return mat;
}

// Fonction pour libérer la mémoire d'une matrice
void liberer_matrice(matrice *a) {
    for (int i = 0; i < a->n; i++) {
        free(a->mat[i]);
    }
    free(a->mat);
    free(a);
}

// Exemple d'application de Gauss-Jordan sur la matrice M donnée
int main() {
    matrice *M = creer_matrice(4, 4);
    
    M->mat[0][0] = 2;  M->mat[0][1] = 4;  M->mat[0][2] = -8;  M->mat[0][3] = 10;
    M->mat[1][0] = -2; M->mat[1][1] = -4; M->mat[1][2] = 8;   M->mat[1][3] = 10;
    M->mat[2][0] = 3;  M->mat[2][1] = 9;  M->mat[2][2] = -3;  M->mat[2][3] = 6;
    M->mat[3][0] = 1;  M->mat[3][1] = 4;  M->mat[3][2] = 2;   M->mat[3][3] = -2;

    printf("Matrice initiale :\n");
    afficher_matrice(M);
    
    Gauss_Jordan(M);

    liberer_matrice(M);
    
    return 0;
}
