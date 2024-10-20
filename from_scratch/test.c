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
int test_matrice() {
    matrice a= matrice_nulle(2,3);
    matrice b= matrice_nulle(3,3);

    a.mat[0][0] = 1; a.mat[0][1] = 3; a.mat[0][2] = 0;
    a.mat[1][0] = 1; a.mat[1][1] = 2; a.mat[1][2] = 1;
    
    b.mat[0][0] = 2; b.mat[0][1] = 0; b.mat[0][2] = 0;
    b.mat[1][0] = 0; b.mat[1][1] = 1; b.mat[1][2] = 1;
    b.mat[2][0] = 1; b.mat[2][1] = 0; b.mat[2][2] = 1;
    
    print_matrice(a);
    print_matrice(b);

/*    matrice c=somme(a,b);
    print_matrice(c);
    free_matrice(c);*/

    matrice d=produit(a,b);
    print_matrice(d);
    free_matrice(d);

    matrice t=transposee(a);
    print_matrice(t);
    free_matrice(a);
    free_matrice(b);
    return 0;
}

/*int test_lecture_point() {
        for (int i = 0; i < n1; i++) {
        printf("Point %d : Caméra 1 -> (%0.1f, %0.1f), Caméra 2 -> (%0.1f, %0.1f)\n",
               i + 1, points[i].cam1.x, points[i].cam1.y,
               points[i].cam2.x, points[i].cam2.y);
    }
    return 0;
}*/
// Exemple d'application de Gauss-Jordan sur la matrice M donnée
int test_Gauss_Jordan() {
    matrice M = matrice_nulle(4, 4);
    
    M.mat[0][0] = 2;  M.mat[0][1] = 4;  M.mat[0][2] = -8;  M.mat[0][3] = 10;
    M.mat[1][0] = -2; M.mat[1][1] = -4; M.mat[1][2] = 8;   M.mat[1][3] = 10;
    M.mat[2][0] = 3;  M.mat[2][1] = 9;  M.mat[2][2] = -3;  M.mat[2][3] = 6;
    M.mat[3][0] = 1;  M.mat[3][1] = 4;  M.mat[3][2] = 2;   M.mat[3][3] = -2;

    printf("Matrice initiale :\n");
    print_matrice(M);   
    Gauss_Jordan(&M);
    print_matrice(M);   
    return 0;
}

int test_inverser() {
    // Exemple de matrice 3x3
    matrice* M = matrice_nulle_pointeur(3, 3);
    M->mat[0][0] = 1;
    M->mat[0][1] = 0;
    M->mat[0][2] = 5;
    M->mat[1][0] = 2;
    M->mat[1][1] = 1;
    M->mat[1][2] = 6;
    M->mat[2][0] = 3;
    M->mat[2][1] = 4;
    M->mat[2][2] = 0;
    
    printf("Matrice M:\n");
    print_matrice(*M);
    
    // Inversion de la matrice
    matrice* inv = inverser_matrice(M);
    
    printf("\nMatrice inverse de M:\n");
    print_matrice(*inv);
    
    return 0;
}
int test_resolution_system() {
    // Définir la matrice A (3x3) comme pointeur
    matrice* A = matrice_nulle_pointeur(3, 3);
    A->mat[0][0] = 1; A->mat[0][1] = 2; A->mat[0][2] = -1;
    A->mat[1][0] = 1; A->mat[1][1] = 1; A->mat[1][2] = -2;
    A->mat[2][0] = 2; A->mat[2][1] = 1; A->mat[2][2] = -1;

    // Définir le vecteur V (3x1) comme pointeur
    matrice* V = matrice_nulle_pointeur(3, 1);
    V->mat[0][0] = 2;
    V->mat[1][0] = -3;
    V->mat[2][0] = 1;

    // Résoudre le système AU = V
    printf("resolution du systeme");
    matrice* U = resolution_systeme(A, V);

    // Afficher la solution U
    print_matrice(*U);  // Déréférencer U pour l'affichage

    // Libérer la mémoire allouée pour U si nécessaire
    // free(U->mat);
    // free(U);

    return 0;
}
matrice calcul_mat() {
    int n;
    FILE *fichier1 = fopen("points_img1.txt", "r");
    fscanf(fichier1, "%d", &n);
    fclose(fichier1);

    Point* points[n];
    for (int i = 0; i < n; ++i){
        points[i]=point_vide_pointeur();
    }
    lirePoints("points_img1.txt", points, n,1);
    lirePoints("points_img2.txt", points, n,2);
    print_point(points[0]);
    matrice C1 = concatenation3(vect3_of_vect2(points[0]->cam1),vect3_of_vect2(points[1]->cam1),vect3_of_vect2(points[2]->cam1));
    matrice C2 = concatenation3(vect3_of_vect2(points[0]->cam2),vect3_of_vect2(points[1]->cam2),vect3_of_vect2(points[2]->cam2));
    matrice C3_1 = vect3_of_vect2(points[3]->cam1);
    matrice C3_2 = vect3_of_vect2(points[3]->cam2);
    printf("C1:\n");
    print_matrice(C1);
    printf("C2:\n");
    print_matrice(C2);
    printf("points[3]->cam1:\n");
    print_matrice(points[3]->cam1);
    matrice* k1=resolution_systeme(&C1,&C3_1);
     printf("test\n");
    matrice* k2=resolution_systeme(&C2,&C3_2);
    printf("k1:\n");
    print_matrice(*k1);
    printf("k2:\n");
    print_matrice(*k2);
    float r1=(k1->mat[0][0])/(k2->mat[0][0]);
    float r2=(k1->mat[1][0])/(k2->mat[1][0]);
    float r3=(k1->mat[2][0])/(k2->mat[2][0]);
    matrice C1_t=transposee(C1);
    matrice* Rx=matrice_nulle_pointeur(3,1);
    Rx->mat[0][0]=r1*C2.mat[0][0];
    Rx->mat[1][0]=r2*C2.mat[0][1];
    Rx->mat[2][0]=r3*C2.mat[0][2];
    matrice* Ry=matrice_nulle_pointeur(3,1);
    Ry->mat[0][0]=r1*C2.mat[1][0];
    Ry->mat[1][0]=r2*C2.mat[1][1];
    Ry->mat[2][0]=r3*C2.mat[1][2];
    matrice* R=matrice_nulle_pointeur(3,1);
    R->mat[0][0]=r1;
    R->mat[1][0]=r2;
    R->mat[2][0]=r3;
    matrice* Mat_c1_t=resolution_systeme(&C1_t,Rx);
    matrice* Mat_c2_t=resolution_systeme(&C1_t,Ry);
    matrice* Mat_c3_t=resolution_systeme(&C1_t,R);
    matrice Mat=transposee(concatenation3(*Mat_c1_t,*Mat_c2_t,*Mat_c3_t));
    print_matrice(Mat);
    return 0;
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

