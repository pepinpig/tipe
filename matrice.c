#include "matrice.h"


matrice* matrice_nulle(int n, int m) {
    matrice* M = (matrice*) malloc(sizeof(matrice));
    assert(M != NULL);
    M->n = n;
    M->m = m;
    M->mat = (double**) malloc(n * sizeof(double*));
    assert(M!=NULL);
    for (int i = 0; i < n; i++) {
        M->mat[i] = (double*) calloc(m, sizeof(double)); 
        assert(M->mat[i]);
    }

    return M;  
}

matrice* matrice_identite(int n) {
    matrice * I = matrice_nulle(n, n);
    for (int i = 0; i < n; ++i) {
        I->mat[i][i] = 1;
    }
    return I;
}

void copie_matrice(matrice* old, matrice* new){
    assert(old->n==new->n);
    assert(old->m==new->m);
    for (int i = 0; i < old->n; i++){
        for (int j = 0; j < old->m; j++){
            new->mat[i][j]=old->mat[i][j];
        }
    }
}

double norme_vecteur(matrice* a, int colonne) {
    double somme = 0;
    for (int i = 0; i < a->n; ++i) {
        somme += a->mat[i][colonne] * a->mat[i][colonne];
    }
    return sqrt(somme);
}

void print_matrice (matrice * a) {
    for(int i = 0; i < a->n; i++){
        for(int j = 0; j < a->m; j++){
            printf("%6.3lf ",a->mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double produit_scalaire(matrice* l, matrice* X2) {
    return l->mat[0][0] * X2->mat[0][0] +
           l->mat[1][0] * X2->mat[1][0] +
           l->mat[2][0] * X2->mat[2][0];
}

bool matrice_egale(matrice* a, matrice* b) {
    if (a->n != b->n || a->m != b->m) {
        return false;
    }

    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < a->m; j++) {
            if (fabs(a->mat[i][j] - b->mat[i][j]) > EPSILON) {
                return false;
            }
        }
    }

    return true;
}

void free_matrice(matrice * a){
    for (int i = 0; i < a->n; i++) {
        free(a->mat[i]);
    }
    free(a->mat);
}

matrice * somme(matrice *a, matrice *b) {
    assert(a->n == b->n);
    assert(a->m == b->m);

    matrice * c = matrice_nulle(a->n,a->m);
    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < a->m; j++) {
            c->mat[i][j] = a->mat[i][j] + b->mat[i][j];
        }
    }
    return c;
}


matrice* produit(matrice* a, matrice* b) {
    assert(a->m == b->n);
    matrice * c = matrice_nulle(a->n, b->m);
    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < b->m; j++) {
            for (int k = 0; k < a->m; k++) {
                c->mat[i][j] += a->mat[i][k] * b->mat[k][j];
            }
        }
    }
    return c;
}


void multiplication_scalaire(matrice* a, double lambda) {
    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < a->m; j++) {
            a->mat[i][j] = lambda*(a->mat[i][j]);
        }
    }
}

matrice* transposee(matrice* a) {
    matrice* at = matrice_nulle(a->m,a->n);
    for (int i = 0; i < a->n; i++) {
        for (int j=0; j<a->m; j++){
            at->mat[j][i]=a->mat[i][j];
        }
    }
    return at;
}


matrice* concatenation2(matrice* a, matrice* b) {
    assert(a->n == b->n);
    matrice* result = matrice_nulle(a->n, a->m + b->m);
    for (int i = 0; i < a->n; i++) {
        for (int j = 0; j < a->m; j++) {
            result->mat[i][j] = a->mat[i][j];
        }
    }
    for (int i = 0; i < b->n; i++) {
        for (int j = 0; j < b->m; j++) {
            result->mat[i][a->m + j] = b->mat[i][j];
        }
    }
    return result;
}

matrice* concatenation3(matrice* a, matrice* b, matrice* c) {
    return concatenation2(concatenation2(a,b),c);
}
matrice* concatenationv2(matrice* a, matrice* b){
    return transposee(concatenation2(transposee(a), transposee(b)));
}

matrice* concatenationv3(matrice* a, matrice*b, matrice* c){
    return concatenationv2(concatenationv2(a,b),c);
}


void assert_ligne_in_range(matrice * a, int i){
    assert(i >= 0 && i < a->n);
}
void assert_colonne_in_range(matrice * a, int j){
    assert(j >= 0 && j < a->m);
}

/*Resolution de système AU=V d inconnue U*/

void echange_ligne(matrice* a, int i, int j) {
    assert_ligne_in_range(a,i);
    assert_colonne_in_range(a,j);

    double * ligne_i =  a->mat[i];
    a->mat[i] = a->mat[j];
    a->mat[j] = ligne_i;
}

void multiplication_ligne(matrice* a, int i, double lambda) {
    assert_ligne_in_range(a,i);
    for (int k = 0; k < a->m; k++) {
        a->mat[i][k] *= lambda;
    }
}

void ajout_ligne(matrice* a, int i1, int i2, double lambda) {
    assert_ligne_in_range(a,i1);
    assert_ligne_in_range(a,i2);
    for (int k = 0; k < a->m; k++) {
        a->mat[i1][k] = a->mat[i1][k] + lambda * a->mat[i2][k];
    }
}

void set_colonne(matrice *target, int col_idx, matrice* column_vector) {
    assert_colonne_in_range(target, col_idx);

    if (column_vector->n != target->n || column_vector->m != 1) {
        fprintf(stderr, "Dimension mismatch in set_colonne\n");
        return;
    }

    for (int i = 0; i < target->n; i++) {
        target->mat[i][col_idx] = column_vector->mat[i][0];
    }
}

matrice* matrice_colonne(matrice* A, int j) {
    assert_colonne_in_range(A, j);
    
    // Création de la matrice colonne (n x 1)
    matrice* col = matrice_nulle(A->n, 1);
    
    // Remplir la matrice colonne avec les éléments de la colonne j de A
    for (int i = 0; i < A->n; i++) {
        col->mat[i][0] = A->mat[i][j];
    }
    
    return col;
}



/*Le pivot partiel consiste à rechercher, dans la colonne en cours 
(à partir de la ligne actuelle et en dessous), l'élément ayant la plus grande valeur absolue, 
puis à permuter les lignes pour que cet élément devienne le pivot.
Cela permet d'éviter les divisions par de petites valeurs, ce qui stabilise numériquement le processus.*/

int choix_pivot_naif(matrice *a, int j) {
    int n = a->n;
    for (int k = j; k < n; k++) {
        if (fabs(a->mat[k][j]) > EPSILON) {
            return k;
        }
    }
    return -1;
}

// Choix du pivot : recherche de l'élément maximum en valeur absolue dans la colonne j à partir de la ligne j
int choix_pivot_partiel(matrice *a, int j) {
    int n = a->n;
    int max_index = j;
    double max_value = fabs(a->mat[j][j]);
    
    for (int i = j + 1; i < n; i++) {
        if(fabs(a->mat[i][j]) < EPSILON){
            a->mat[i][j] = 0;
        }
        if (fabs(a->mat[i][j]) > max_value) {
            max_value = fabs(a->mat[i][j]);
            max_index = i;
        }
    }
    if (max_value == 0) {
        return -1;  // Retourne -1 si aucun pivot n'est trouvé
    }
    return max_index;
}


// Algorithme de Gauss-Jordan
void Gauss_Jordan(matrice* a) {
    int n = a->n; // nombre de ligne
    int r = a->m;
    if (r > n){ r = n ;}
    // r = min(n, m)
    for (int j = 0; j < n; ++j) { 
        int pivot_index = choix_pivot_partiel(a, j);  // Choix du pivot
        if (pivot_index != -1) {
            echange_ligne(a, j, pivot_index);  // Remontée de la ligne du pivot
            double pivot_value = a->mat[j][j];  // Unitarisation du pivot
            multiplication_ligne(a, j, 1. / pivot_value);  
            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {  // Utiliser n pour itérer sur les lignes
                if (k != j) {
                    double lambda = -a->mat[k][j];
                    ajout_ligne(a, k, j, lambda); 
                }
            }
        } 
    }
}

// Fonction Gauss-Jordan avec affichage réduit pour débogage

// Fonction principale de Gauss-Jordan
void Gauss_Jordan_print(matrice* a) {
    int n = a->n; // nombre de lignes
    int m = a->m; // nombre de colonnes
    int r = m;
    if (r > n){ r = n ;}
    // r = min(n, m)

    for (int j = 0; j < r; ++j) {  // Itération sur les colonnes
        // Choix du pivot
        int pivot_index = choix_pivot_partiel(a, j);
        if (pivot_index != -1) {
            // Remontée de la ligne du pivot si nécessaire
            if (pivot_index != j) {
                echange_ligne(a, j, pivot_index);
            }

            // Unitarisation du pivot
            double pivot_value = a->mat[j][j];
            if (pivot_value != 0) { // Évite la division par zéro
                multiplication_ligne(a, j, 1. / pivot_value);
            }


            // Élimination dans les autres lignes
            for (int k = 0; k < n; ++k) {
                if (k != j && j < m) { // Assurer que j est dans les limites des colonnes
                    double lambda = -a->mat[k][j];
                    if (lambda != 0) {
                        ajout_ligne(a, k, j, lambda);
                    }
                }
            }
            print_matrice(a);
        }
    }
    print_matrice(a);
}

matrice* inverser_matrice(matrice* a) {
    if (a->n != a->m) {
        printf("Erreur : La matrice n'est pas carrée, donc non inversible.\n");
        exit(EXIT_FAILURE);
    }

    int n = a->n;  // Nombre de lignes (ou colonnes, car carrée)
    matrice* inv = matrice_nulle(n, n);  // Crée une matrice pour l'inverse
    
    // Initialisation de la matrice identité
    for (int i = 0; i < n; i++) {
        inv->mat[i][i] = 1;
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
        double pivot_value = a->mat[j][j];
        if (fabs(pivot_value) < EPSILON) {  // Gestion d'une valeur pivot trop petite
            printf("Erreur : Pivot presque nul, la matrice est singulière->\n");
            exit(EXIT_FAILURE);
        }

        multiplication_ligne(a, j, 1. / pivot_value);
        multiplication_ligne(inv, j, 1. / pivot_value);

        // Élimination des autres lignes
        for (int k = 0; k < n; ++k) {
            if (k != j) {
                double lambda = -a->mat[k][j];
                ajout_ligne(a, k, j, lambda);
                ajout_ligne(inv, k, j, lambda);
            }
        }
    }

    return inv;
}



matrice* pseudo_inverser_matrice(matrice* a) {  
    matrice* at = transposee(a);
    matrice* at_a = produit(at,a);
    matrice* at_a_inv = inverser_matrice(at_a);
    matrice* pseudoinv = produit(at_a_inv,at);
    return pseudoinv;
}


matrice* resolution_systeme(matrice* A, matrice* V) {
    int n = A->n;
    int m = A->m;
    // Création de la matrice augmentée [A | V]
    matrice* Augmente = matrice_nulle(n, m + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Augmente->mat[i][j] = A->mat[i][j];
        }
        Augmente->mat[i][m] = V->mat[i][0];  // Colonne augmentée avec V
    }
    Gauss_Jordan(Augmente);
    // Extrairere la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle(n, 1);
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
    print_matrice(A);  // Print de la matrice A

    printf("Vecteur V initial :\n");
    print_matrice(V);  // Print du vecteur V

    // Création de la matrice augmentée [A | V]
    matrice* Augmente = matrice_nulle(n, m + 1);  // A avec une colonne supplémentaire

    if (Augmente == NULL) {
        printf("Erreur d'allocation mémoire pour la matrice augmentée->\n");
        exit(1);  // Sortir si l'allocation échoue
    }

    printf("Création de la matrice augmentée [A | V]->->->\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Augmente->mat[i][j] = A->mat[i][j];  // Copier chaque élément de A
        }
        
        // Vérification des dimensions de V
        if (i < V->n && V->m == 1) {  // V est supposé être un vecteur colonne
            Augmente->mat[i][m] = V->mat[i][0];  // Ajouter V comme colonne supplémentaire
        } else {
            printf("Erreur : Incohérence de dimensions entre A et V->\n");
            exit(1);  // Sortir si les dimensions ne sont pas compatibles
        }
    }

    printf("Matrice augmentée [A | V] avant Gauss-Jordan :\n");
    print_matrice(Augmente);  // Print de la matrice augmentée

    // Appliquer Gauss-Jordan
    Gauss_Jordan_print(Augmente);  // Il est supposé que cette fonction modifie la matrice augmentée

    printf("Matrice augmentée [A | V] après Gauss-Jordan :\n");
    print_matrice(Augmente);  // Print de la matrice augmentée après Gauss-Jordan

    // Extraire la solution U (les colonnes après la matrice A)
    matrice* U = matrice_nulle(n, 1);
    printf("Extraction de la solution U :\n");
    for (int i = 0; i < n; i++) {
        U->mat[i][0] = Augmente->mat[i][m];
        printf("U[%d][0] = %lf\n", i, U->mat[i][0]);  // Print chaque élément de la solution U
    }

    // Libération de la mémoire
    free(Augmente->mat);
    free(Augmente);
    
    return U;
}

matrice* produit_vectoriel(matrice* v, matrice* b){
    matrice* ax = matrice_nulle(3,3);
    ax->mat[0][1]=-(v->mat[3][0]);
    ax->mat[0][2]=(v->mat[2][0]);
    ax->mat[1][0]=(v->mat[3][0]);
    ax->mat[1][2]=-(v->mat[1][0]);
    ax->mat[2][0]=-(v->mat[2][0]);
    ax->mat[2][1]=(v->mat[1][0]);
    matrice* res = matrice_nulle(3,3);
    res = produit(ax, b);
    return res;
}

matrice* coo_vect(double x, double y){
    matrice* res=matrice_nulle(3,1);
    res->mat[0][0]=x;
    res->mat[1][0]=y;
    res->mat[2][0]=1;
    return res;
}
matrice* coo_vect_inv(double x, double y){
    matrice* res=matrice_nulle(3,1);
    res->mat[0][0]=y;
    res->mat[1][0]=x;
    res->mat[2][0]=1;
    return res;
}
double determinant_matrice(matrice* M) {
    assert(M->n == M->m); // Matrice carrée

    int n = M->n;

    // Cas de base
    if (n == 1) {
        return M->mat[0][0];
    } else if (n == 2) {
        return M->mat[0][0] * M->mat[1][1] - M->mat[0][1] * M->mat[1][0];
    }

    double det = 0.0;

    for (int col = 0; col < n; col++) {
        // Créer la matrice mineure (sans la première ligne et la colonne 'col')
        matrice* minor = matrice_nulle(n - 1, n - 1);
        for (int i = 1; i < n; i++) {
            int minor_col = 0;
            for (int j = 0; j < n; j++) {
                if (j == col) continue;
                minor->mat[i - 1][minor_col] = M->mat[i][j];
                minor_col++;
            }
        }

        // Développement par la 1re ligne
        double coeff = M->mat[0][col];
        double minor_det = determinant_matrice(minor);
        if (col % 2 == 0)
            det += coeff * minor_det;
        else
            det -= coeff * minor_det;

        free_matrice(minor);
    }

    return det;
}
