#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include <math.h>
#define MAX_ROWS 100
#define MAX_COLS 100
#define EPSILON 1e-9 // Pour vérifier si un nombre est proche de zéro


matrice matrice_nulle(int n, int m){
    matrice a;
    a.n = n;
    a.m = m;
    a.mat = calloc(a.n,sizeof(float*));
    for (int i = 0; i < a.n; i++) {
        a.mat[i] = calloc(a.m,sizeof(float));
    }
    return a;
}

matrice* matrice_nulle_pointeur(int n, int m) {
    matrice* M = (matrice*) malloc(sizeof(matrice));  // Alloue de l'espace pour la structure matrice
    M->n = n;
    M->m = m;
    
    // Alloue de l'espace pour les lignes de la matrice
    M->mat = (float**) malloc(n * sizeof(float*));
    for (int i = 0; i < n; i++) {
        M->mat[i] = (float*) calloc(m, sizeof(float));  // Remplir chaque ligne avec des zéros
    }

    return M;  // Retourne un pointeur vers la matrice
}


void print_matrice (matrice a) {
    for(int i=0; i<a.n; i++){
        for(int j=0;j<a.m;j++){
            printf("%6.2f ",a.mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_matrice(matrice a){
    for (int i = 0; i < a.n; i++) {
        free(a.mat[i]);
    }
    free(a.mat);
}

matrice somme(matrice a, matrice b) {
    assert(a.n == b.n);
    assert(a.m == b.m);

    matrice c = matrice_nulle(a.n,a.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            c.mat[i][j] = a.mat[i][j] + b.mat[i][j];
        }
    }

    return c;
}


matrice produit(matrice a, matrice b) {
    assert(a.m == b.n);
    matrice c = matrice_nulle(a.n, b.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < b.m; j++) {
            for (int k = 0; k < a.m; k++) {
                c.mat[i][j] += a.mat[i][k] * b.mat[k][j];
            }
        }
    }

    return c;
}


void multiplication_scalaire(matrice a, float lambda) {
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            a.mat[i][j] = lambda*(a.mat[i][j]);
        }
    }
}

matrice transposee(matrice a) {
    matrice at = matrice_nulle(a.m,a.n);
    for (int i = 0; i < a.n; i++) {
        for (int j=0; j<a.m; j++){
            at.mat[j][i]=a.mat[i][j];
        }
    }
    return at;
}


matrice concatenation2(matrice a, matrice b) {
    assert(a.n == b.n);
    matrice result = matrice_nulle(a.n, a.m + b.m);
    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.m; j++) {
            result.mat[i][j] = a.mat[i][j];
        }
    }
    for (int i = 0; i < b.n; i++) {
        for (int j = 0; j < b.m; j++) {
            result.mat[i][a.m + j] = b.mat[i][j];
        }
    }
    return result;
}

matrice concatenation3(matrice a, matrice b, matrice c) {
    concatenation2(concatenation2(a,b),c);
}



/*Resolution de système AU=V d inconnue U*/

void echange_ligne(matrice* a, int i, int j) {
    assert(i < a->n);
    assert(j < a->n);
    for (int k = 0; k < a->m; k++) {
        float temp = a->mat[i][k];
        a->mat[i][k] = a->mat[j][k];
        a->mat[j][k] = temp;
    }
}

void multiplication_ligne(matrice* a, int i, float lambda) {
    assert(i < a->n);
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] = lambda * a->mat[i][k];
    }
}

void ajout_ligne(matrice* a, int i, int j, float lambda) {
    assert(i < a->n);
    assert(j < a->n);
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] = a->mat[i][k] + lambda * a->mat[j][k];
    }
}


/*Le pivot partiel consiste à rechercher, dans la colonne en cours 
(à partir de la ligne actuelle et en dessous), l'élément ayant la plus grande valeur absolue, 
puis à permuter les lignes pour que cet élément devienne le pivot. 
Cela permet d'éviter les divisions par de petites valeurs, ce qui stabilise numériquement le processus.*/

int choix_pivot_naïf(matrice *a, int i) {
    int n = a->n;
    for (int k = i; k < n; k++) {
        if (fabs(a->mat[k][i]) > 0) {
            return k;
        }
    }
    return -1;
}

// Choix du pivot : recherche de l'élément maximum en valeur absolue dans la colonne j à partir de la ligne i
int choix_pivot_partiel(matrice *a, int i) {
    int n = a->n;
    int m = a->m;
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


// Algorithme de Gauss-Jordan
int Gauss_Jordan(matrice* a) {
    int n = a->n; //nombre de ligne
    int m = a->m; //nombre de colonne
    for (int j = 0; j < n; ++j) { 
        int pivot_index = choix_pivot_partiel(a, j);  // Choix du pivot
        if (pivot_index != -1) {
            echange_ligne(a, j, pivot_index);  // Remontée de la ligne du pivot
            float pivot_value = a->mat[j][j];  // Unitarisation du pivot
            multiplication_ligne(a, j, 1.0f / pivot_value);  
            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {  // Utiliser n pour itérer sur les lignes
                if (k != j) {
                    float lambda = -a->mat[k][j];
                    ajout_ligne(a, k, j, lambda); 
                }
            }
        } 
    }
    return 0;
}



matrice* inverser_matrice(matrice* a) {
    int n = a->n; //nombre de ligne
    int m = a->m; //nombre de colonne
    matrice* inv = matrice_nulle_pointeur(n, n);  // Crée une matrice identité pour l'inverse
    for (int i = 0; i < n; i++) {
        inv->mat[i][i] = 1;  // Initialisation de la matrice identité
    }
    for (int j = 0; j < m; ++j) { 
        int pivot_index = choix_pivot_partiel(a, j);  // Choix du pivot
        if (pivot_index != -1) {
            echange_ligne(a, j, pivot_index);  // Remontée de la ligne du pivot
            echange_ligne(inv, j, pivot_index);  // Remontée de la ligne du pivot
            float pivot_value = a->mat[j][j];  // Unitarisation du pivot
            multiplication_ligne(a, j, 1.0f / pivot_value);  
            multiplication_ligne(inv, j, 1.0f / pivot_value);
            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {  // Utiliser n pour itérer sur les lignes
                if (k != j) {
                    float lambda = -a->mat[k][j];
                    ajout_ligne(a, k, j, lambda); 
                    ajout_ligne(inv, k, j, lambda); 
                }
            }
        }
        else{
            printf("Erreur : la matrice est non inversible.\n");
            exit(EXIT_FAILURE);
        } 
    }
    return inv;
}

matrice* resolution_systeme(matrice* A, matrice* V) {
    int n = A->n;
    int m = A->m;
    // Création de la matrice augmentée [A | V]
    matrice* Augmente = matrice_nulle_pointeur(n, m + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Augmente->mat[i][j] = A->mat[i][j];
        }
        Augmente->mat[i][m] = V->mat[i][0];  // Colonne augmentée avec V
    }
    Gauss_Jordan(Augmente);
    // Extraire la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle_pointeur(n, 1);
    for (int i = 0; i < n; i++) {
        U->mat[i][0] = Augmente->mat[i][n];
    }

    return U;
}

matrice* resolution_systeme_print(matrice* A, matrice* V) {
    int n = A->n;
    int m = A->m;

    printf("Matrice A initiale :\n");
    print_matrice(*A);  // Print de la matrice A

    printf("Vecteur V initial :\n");
    print_matrice(*V);  // Print du vecteur V

    // Création de la matrice augmentée [A | V]
    // Création de la matrice augmentée [A | V]
    matrice* Augmente = matrice_nulle_pointeur(n, m + 1);  // A avec une colonne supplémentaire

    if (Augmente == NULL) {
        printf("Erreur d'allocation mémoire pour la matrice augmentée.\n");
        exit(1);  // Sortir si l'allocation échoue
    }

    printf("Création de la matrice augmentée [A | V]...\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Augmente->mat[i][j] = A->mat[i][j];  // Copier chaque élément de A
        }
        
        // Vérification des dimensions de V
        if (i < V->n && V->m == 1) {  // V est supposé être un vecteur colonne
            Augmente->mat[i][m] = V->mat[i][0];  // Ajouter V comme colonne supplémentaire
        } else {
            printf("Erreur : Incohérence de dimensions entre A et V.\n");
            exit(1);  // Sortir si les dimensions ne sont pas compatibles
        }
    }

    printf("Matrice augmentée [A | V] avant Gauss-Jordan :\n");
    print_matrice(*Augmente);  // Print de la matrice augmentée

    // Appliquer Gauss-Jordan
    Gauss_Jordan(Augmente);  // Il est supposé que cette fonction modifie la matrice augmentée

    printf("Matrice augmentée [A | V] après Gauss-Jordan :\n");
    print_matrice(*Augmente);  // Print de la matrice augmentée après Gauss-Jordan

    // Extraire la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle_pointeur(n, 1);
    printf("Extraction de la solution U :\n");
    for (int i = 0; i < n; i++) {
        U->mat[i][0] = Augmente->mat[i][m];
        printf("U[%d][0] = %f\n", i, U->mat[i][0]);  // Print chaque élément de la solution U
    }

    return U;
}
