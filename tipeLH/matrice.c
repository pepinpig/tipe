#include "matrice.h"

#define MAX_ROWS 100
#define MAX_COLS 100
#define EPSILON 1e-9 // Pour vérifier si un nombre est proche de zéro


matrice matrice_nulle(int n, int m){
    matrice a;
    a.n = n;
    a.m = m;
    a.mat = calloc(a.n,sizeof(long double*));
    for (int i = 0; i < a.n; i++) {
        a.mat[i] = calloc(a.m,sizeof(long double));
    }
    return a;
}

matrice* matrice_nulle_pointeur(int n, int m) {
    matrice* M = (matrice*) malloc(sizeof(matrice));  
    M->n = n;
    M->m = m;
    
    M->mat = (long double**) malloc(n * sizeof(long double*));
    for (int i = 0; i < n; i++) {
        M->mat[i] = (long double*) calloc(m, sizeof(long double)); 
    }

    return M;  
}

matrice matrice_identite(int n) {
    matrice I = matrice_nulle(n, n);
    for (int i = 0; i < n; ++i) {
        I.mat[i][i] = 1.0;
    }
    return I;
}

long double norme_vecteur(matrice a, int colonne) {
    long double somme = 0.0;
    for (int i = 0; i < a.n; ++i) {
        somme += a.mat[i][colonne] * a.mat[i][colonne];
    }
    return sqrt(somme);
}
void print_matrice (matrice a) {
    for(int i=0; i<a.n; i++){
        for(int j=0;j<a.m;j++){
            printf("%6.3Lf ",a.mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


bool matrice_egale(matrice a, matrice b) {
    long double tol = 0.1; 

    if (a.n != b.n || a.m != b.m) {
        return false;
    }
    for (int i = 0; i < a.n; ++i) {
        for (int j = 0; j < a.m; ++j) {
            if (fabs(a.mat[i][j] - b.mat[i][j]) > tol) {
                printf("A[%d][%d]=%Lf not equal to B[%d][%d]=%Lf\n",
                       i, j, a.mat[i][j], i, j, b.mat[i][j]);
                return false;
            }
        }
    }

    return true;
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

matrice copie(matrice a){
    matrice res=matrice_nulle(a.m,a.n);
    for (int i = 0; i < a.n; i++) {
        for (int j=0; j<a.m; j++){
            res.mat[i][j]=a.mat[i][j];
        }
    }
    return res;
}

void multiplication_scalaire(matrice a, long double lambda) {
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
    return concatenation2(concatenation2(a,b),c);
}



/*Resolution de système AU=V d inconnue U*/

void echange_ligne(matrice* a, int i, int j) {
    assert(i < a->n);
    assert(j < a->n);
    for (int k = 0; k < a->m; k++) {
        long double temp = a->mat[i][k];
        a->mat[i][k] = a->mat[j][k];
        a->mat[j][k] = temp;
    }
}

void multiplication_ligne(matrice* a, int i, long double lambda) {
    assert(i < a->n);
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] = lambda * a->mat[i][k];
    }
}

void ajout_ligne(matrice* a, int i, int j, long double lambda) {
    assert(i < a->n);
    assert(j < a->n);
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] = a->mat[i][k] + lambda * a->mat[j][k];
    }
}

void set_colonne(matrice *target, int col_idx, matrice column_vector) {
    // Ensure column index is within bounds
    if (col_idx < 0 || col_idx >= target->m) {
        fprintf(stderr, "Column index out of bounds in set_colonne.\n");
        return;
    }

    // Ensure the dimensions of the column vector are compatible
    if (column_vector.n != target->n || column_vector.m != 1) {
        fprintf(stderr, "Dimension mismatch in set_colonne.\n");
        return;
    }

    // Replace the column values
    for (int i = 0; i < target->n; i++) {
        target->mat[i][col_idx] = column_vector.mat[i][0];
    }
}

matrice matrice_colonne(matrice A, int i) {
    // Vérification de la validité de l'indice i
    if (i < 0 || i >= A.m) {
        printf("Indice de colonne invalide\n");
        exit(1);
    }
    
    // Création de la matrice colonne (n x 1)
    matrice col = matrice_nulle(A.n, 1);
    
    // Remplir la matrice colonne avec les éléments de la colonne i de A
    for (int j = 0; j < A.n; j++) {
        col.mat[j][0] = A.mat[j][i];
    }
    
    return col;
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
    int max_index = i;
    long double max_value = fabs(a->mat[i][i]);
    
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
    for (int j = 0; j < n; ++j) { 
        int pivot_index = choix_pivot_partiel(a, j);  // Choix du pivot
        if (pivot_index != -1) {
            echange_ligne(a, j, pivot_index);  // Remontée de la ligne du pivot
            long double pivot_value = a->mat[j][j];  // Unitarisation du pivot
            multiplication_ligne(a, j, 1.0f / pivot_value);  
            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {  // Utiliser n pour itérer sur les lignes
                if (k != j) {
                    long double lambda = -a->mat[k][j];
                    ajout_ligne(a, k, j, lambda); 
                }
            }
        } 
    }
    return 0;
}





// Fonction Gauss-Jordan avec affichage réduit pour débogage

// Fonction principale de Gauss-Jordan
int Gauss_Jordan_print(matrice* a) {
    int n = a->n; // nombre de lignes
    int m = a->m; // nombre de colonnes

    for (int j = 0; j < m; ++j) {  // Itération sur les colonnes
        if (j >= n) {
            break;  // Si j dépasse le nombre de lignes, on arrête
        }

        // Choix du pivot
        int pivot_index = choix_pivot_partiel(a, j);
        if (pivot_index != -1) {
            // Remontée de la ligne du pivot si nécessaire
            if (pivot_index != j) {
                echange_ligne(a, j, pivot_index);
            }

            // Unitarisation du pivot
            long double pivot_value = a->mat[j][j];
            if (pivot_value != 0) { // Évite la division par zéro
                multiplication_ligne(a, j, 1.0f / pivot_value);
            }


            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {
                if (k != j && j < m) { // Assurer que j est dans les limites des colonnes
                    long double lambda = -a->mat[k][j];
                    if (lambda != 0) {
                        ajout_ligne(a, k, j, lambda);
                    }
                }
            }
            print_matrice(*a);
        }
    }
    print_matrice(*a);

    return 0;
}
matrice* inverser_matrice(matrice* a) {
    if (a->n != a->m) {
        printf("Erreur : La matrice n'est pas carrée, donc non inversible.\n");
        exit(EXIT_FAILURE);
    }

    int n = a->n;  // Nombre de lignes (ou colonnes, car carrée)
    matrice* inv = matrice_nulle_pointeur(n, n);  // Crée une matrice pour l'inverse
    
    // Initialisation de la matrice identité
    for (int i = 0; i < n; i++) {
        inv->mat[i][i] = 1.0;
    }

    for (int j = 0; j < n; ++j) { 
        // Choix du pivot : recherche du maximum en valeur absolue
        int pivot_index = choix_pivot_partiel(a, j);
        if (pivot_index == -1) {
            printf("Erreur : la matrice est singulière, donc non inversible.\n");
            exit(EXIT_FAILURE);
        }

        // Échange des lignes si nécessaire
        if (pivot_index != j) {
            echange_ligne(a, j, pivot_index);
            echange_ligne(inv, j, pivot_index);
        }

        // Unitarisation de la ligne du pivot
        long double pivot_value = a->mat[j][j];
        if (fabs(pivot_value) < 1e-12) {  // Gestion d'une valeur pivot trop petite
            printf("Erreur : Pivot presque nul, la matrice est singulière.\n");
            exit(EXIT_FAILURE);
        }

        multiplication_ligne(a, j, 1.0 / pivot_value);
        multiplication_ligne(inv, j, 1.0 / pivot_value);

        // Élimination des autres lignes
        for (int k = 0; k < n; ++k) {
            if (k != j) {
                long double lambda = -a->mat[k][j];
                ajout_ligne(a, k, j, lambda);
                ajout_ligne(inv, k, j, lambda);
            }
        }
    }

    return inv;
}



matrice pseudo_inverser_matrice(matrice* a) {  
    matrice at=transposee(*a);
    matrice at_a=produit(at,*a);
    matrice* at_a_inv=inverser_matrice(&at_a);
    matrice pseudoinv=produit(*at_a_inv,at);
    return pseudoinv;
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
    // Extrairere la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle_pointeur(n, 1);
    for (int i = 0; i < n; i++) {
        U->mat[i][0] = Augmente->mat[i][n];
    }

    return U;
}


// Fonction principale pour résoudre un système d'équations
matrice* resolution_systeme_print(matrice* A, matrice* V) {
    int n = A->n;
    int m = A->m;

    printf("Matrice A initiale :\n");
    print_matrice(*A);  // Print de la matrice A

    printf("Vecteur V initial :\n");
    print_matrice(*V);  // Print du vecteur V

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
    Gauss_Jordan_print(Augmente);  // Il est supposé que cette fonction modifie la matrice augmentée

    printf("Matrice augmentée [A | V] après Gauss-Jordan :\n");
    print_matrice(*Augmente);  // Print de la matrice augmentée après Gauss-Jordan

    // Extraire la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle_pointeur(n, 1);
    printf("Extraction de la solution U :\n");
    for (int i = 0; i < n; i++) {
        U->mat[i][0] = Augmente->mat[i][m];
        printf("U[%d][0] = %Lf\n", i, U->mat[i][0]);  // Print chaque élément de la solution U
    }

    // Libération de la mémoire
    free(Augmente->mat);
    free(Augmente);
    
    return U;
}
/*

// Fonction pour ajouter une petite perturbation aléatoire
void perturber_matrice(matrice* A, long double epsilon) {
    for (int i = 0; i < A->n; i++) {
        for (int j = 0; j < A->m; j++) {
            A->mat[i][j] += ((long double)rand() / (long double)RAND_MAX) * epsilon;
        }
    }
}

matrice* resolution_systeme_non_nulle(matrice* A, matrice* V) {
    int n = A->n;  // Nombre de lignes
    int m = A->m;  // Nombre de colonnes

    // Création de la matrice augmentée [A | V]
    matrice* Augmente = matrice_nulle_pointeur(n, m + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Augmente->mat[i][j] = A->mat[i][j];
        }
        Augmente->mat[i][m] = V->mat[i][0];  // Colonne augmentée avec V
    }

    // Résolution avec Gauss-Jordan
    if (!Gauss_Jordan(Augmente)) {
        printf("Erreur: La matrice est singulière ou il y a une division par zéro.\n");
        return NULL;
    }

    // Extraire la solution U (la dernière colonne après la matrice A)
    matrice* U = matrice_nulle_pointeur(m, 1);
    int solution_nulle = 1;
    for (int i = 0; i < m; i++) {
        U->mat[i][0] = Augmente->mat[i][m];
        if (U->mat[i][0] != 0) {
            solution_nulle = 0;
        }
    }

    // Si la solution est nulle, trouver une solution non nulle
    if (solution_nulle) {
        printf("Solution nulle trouvée, recherche d'une solution non nulle...\n");
        srand(time(NULL));  // Initialisation de la graine aléatoire
        int tentative = 0;
        long double epsilon = 1e-5;  // Petite perturbation

        // Tentatives de trouver une solution non nulle
        while (solution_nulle && tentative < 10) {
            // Perturber légèrement la matrice A
            perturber_matrice(A, epsilon);

            // Réinitialiser la matrice augmentée [A | V] avec la matrice perturbée
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    Augmente->mat[i][j] = A->mat[i][j];
                }
                Augmente->mat[i][m] = V->mat[i][0];
            }

            // Résolution avec Gauss-Jordan
            if (!Gauss_Jordan(Augmente)) {
                printf("Erreur: La matrice est singulière après perturbation.\n");
                return NULL;
            }

            // Extraire la nouvelle solution
            solution_nulle = 1;
            for (int i = 0; i < m; i++) {
                U->mat[i][0] = Augmente->mat[i][m];
                if (U->mat[i][0] != 0) {
                    solution_nulle = 0;
                }
            }

            tentative++;
        }

        if (solution_nulle) {
            printf("Impossible de trouver une solution non nulle après plusieurs tentatives.\n");
            return NULL;
        }
    }

    return U;
}*/

