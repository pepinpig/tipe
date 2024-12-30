#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrice.h" // Assurez-vous d'inclure les fonctions pour manipuler la structure matrice
#define NUM_POINTS 6

typedef struct {
    matrice U;
    matrice D;
    matrice V;
} SVD_Result;

// Fonction SIGN pour le signe d'un nombre
#define SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define TOL 1e-6 // Tolérance pour vérifier la singularité des valeurs

SVD_Result mtxSVD(matrice* a) {
    int m = a->m;
    int n = a->n;
    int flag, i, its, j, jj, k, l, nm;
    float anorm, c, f, g, h, s, scale, x, y, z;
    
    // Initialisation des matrices
    matrice U = matrice_nulle(m, n);
    matrice V = matrice_nulle(n, n);
    matrice D = matrice_nulle(1, n);  // Utilisée comme vecteur pour stocker les valeurs singulières

    float* tmp = (float*)malloc(n * sizeof(float));
    if (tmp == NULL) {
        printf("Erreur d'allocation mémoire pour le tableau temporaire.\n");
        exit(1);
    }

    // Copie de la matrice a dans U
    for (i = 0; i < m; i++)
        for (j = 0; j < n; j++)
            U.mat[i][j] = a->mat[i][j];

    // Initialisation des variables
    g = scale = anorm = 0.0;
    for (i = 0; i < n; i++) {
        l = i + 2;
        tmp[i] = scale * g;
        g = s = scale = 0.0;

        if (i < m) {
            for (k = i; k < m; k++) scale += fabs(U.mat[k][i]);
            if (scale != 0.0) {
                for (k = i; k < m; k++) {
                    U.mat[k][i] /= scale;
                    s += U.mat[k][i] * U.mat[k][i];
                }
                f = U.mat[i][i];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                U.mat[i][i] = f - g;
                for (j = l - 1; j < n; j++) {
                    for (s = 0.0, k = i; k < m; k++) s += U.mat[k][i] * U.mat[k][j];
                    f = s / h;
                    for (k = i; k < m; k++) U.mat[k][j] += f * U.mat[k][i];
                }
                for (k = i; k < m; k++) U.mat[k][i] *= scale;
            }
        }
        D.mat[0][i] = scale * g;
        g = s = scale = 0.0;
        if (i + 1 <= m && i + 1 != n) {
            for (k = l - 1; k < n; k++) scale += fabs(U.mat[i][k]);
            if (scale != 0.0) {
                for (k = l - 1; k < n; k++) {
                    U.mat[i][k] /= scale;
                    s += U.mat[i][k] * U.mat[i][k];
                }
                f = U.mat[i][l - 1];
                g = -SIGN(sqrt(s), f);
                h = f * g - s;
                U.mat[i][l - 1] = f - g;
                for (k = l - 1; k < n; k++) tmp[k] = U.mat[i][k] / h;
                for (j = l - 1; j < m; j++) {
                    for (s = 0.0, k = l - 1; k < n; k++) s += U.mat[j][k] * U.mat[i][k];
                    for (k = l - 1; k < n; k++) U.mat[j][k] += s * tmp[k];
                }
                for (k = l - 1; k < n; k++) U.mat[i][k] *= scale;
            }
        }
        anorm = (anorm > (fabs(D.mat[0][i]) + fabs(tmp[i]))) ? anorm : (fabs(D.mat[0][i]) + fabs(tmp[i]));
    }

    // Code supplémentaire pour calculer V et affiner les valeurs singulières
    // (similaire à la version originale mais en utilisant la structure matrice)
    // Le code doit être adapté pour remplir les matrices U, D, et V

    // Nettoyage de la mémoire temporaire
    free(tmp);

    // Retour des résultats dans une structure
    SVD_Result result = {U, D, V};
    return result;
}

// Fonction pour effectuer la SVD et calculer la solution non triviale de Ax = 0
float mtxSVDAx0(matrice* a, matrice* x) {
    SVD_Result svd = mtxSVD(a);  // Utilise la nouvelle version qui calcule U, D et V

    // Initialisation des variables pour analyser les valeurs singulières
    float wmax = svd.D.mat[0][0];
    float wmin = svd.D.mat[0][0];
    int jmin = 0;

    // Recherche des valeurs singulières maximales et minimales
    for (int j = 1; j < a->n; j++) {
        if (svd.D.mat[0][j] < wmin) {
            wmin = svd.D.mat[0][j];
            jmin = j;
        }
        if (svd.D.mat[0][j] > wmax) {
            wmax = svd.D.mat[0][j];
        }
    }

    // Test pour la deuxième plus petite valeur singulière (wmin2)
    float wmin2 = wmax;
    for (int j = 0; j < a->n; j++) {
        if (j == jmin) continue;
        if (svd.D.mat[0][j] < wmin2) {
            wmin2 = svd.D.mat[0][j];
        }
    }

    // Copie de la colonne de V correspondant à la plus petite valeur singulière de A dans x
    for (int i = 0; i < a->n; i++) {
        x->mat[i][0] = svd.V.mat[i][jmin];
    }


    // Libération de la mémoire pour les matrices U, D, V
    free_matrice(svd.U);
    free_matrice(svd.D);
    free_matrice(svd.V);

    // Retourne l'erreur (ratio entre la plus petite et la plus grande valeur singulière)
    return (wmax > TOL) ? (wmin / wmax) : 0.0;
}
float mtxSVDAx0_print(matrice* a, matrice* x) {
    // Calcul de la décomposition SVD
    printf("Début du calcul SVD...\n");
    SVD_Result svd = mtxSVD(a);  // Utilise la nouvelle version qui calcule U, D et V

    // Affichage des matrices issues de la SVD pour débogage
    printf("Matrice U après SVD :\n");
    print_matrice(svd.U);
    
    printf("Matrice D (valeurs singulières) après SVD :\n");
    print_matrice(svd.D);
    
    printf("Matrice V après SVD :\n");
    print_matrice(svd.V);

    // Initialisation des variables pour analyser les valeurs singulières
    float wmax = svd.D.mat[0][0];
    float wmin = svd.D.mat[0][0];
    int jmin = 0;

    // Recherche des valeurs singulières maximales et minimales
    printf("Recherche des valeurs singulières maximales et minimales...\n");
    for (int j = 1; j < a->n; j++) {
        printf("Valeur singulière D[0][%d] = %Lf\n", j, svd.D.mat[0][j]);
        printf("Valeur singulière V[%d][0] = %Lf\n", j, svd.V.mat[j][6]);
        if (svd.D.mat[0][j] < wmin) {
            wmin = svd.D.mat[0][j];
            jmin = j;
        }
        if (svd.D.mat[0][j] > wmax) {
            wmax = svd.D.mat[0][j];
        }
    }

    printf("Valeur singulière minimale trouvée : %Lf (indice %d)\n", wmin, jmin);
    printf("Valeur singulière maximale trouvée : %Lf\n", wmax);

    // Vérification de la validité de jmin
    if (jmin < 0 || jmin >= svd.V.n) {
        printf("Erreur : jmin (%d) est hors des limites de la matrice V.\n", jmin);
        return -1;
    }

    // Copie de la colonne de V correspondant à la plus petite valeur singulière de A dans x
    printf("Copie de la colonne de V correspondant à la plus petite valeur singulière dans x...\n");
    for (int i = 0; i < a->n; i++) {
        x->mat[i][0] = svd.V.mat[i][jmin];
        printf("x->mat[%d][0] = %Lf (copié de V[%d][%d])\n", i, x->mat[i][0], i, jmin);
    }

    printf("Fin du calcul SVD.\n");

    // Libération des ressources SVD
    free_matrice(svd.U);
    free_matrice(svd.D);
    free_matrice(svd.V);

    return 0;
}
    

// Fonction pour charger les points depuis un fichier
void load_points_images(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float u; float v;
    for (int j =0; j < 6; j++) {
        fscanf(file, "%Lf %Lf", &u, &v);
        variables->mat[3*j][12+j]=u;
        variables->mat[3*j+1][12+j]=v;
        variables->mat[3*j+2][12+j]=1;
    }
    fclose(file);
}
// Fonction pour charger les points depuis un fichier
void load_points_reels(const char* filename, matrice* variables, int position) {
    FILE *file = fopen(filename, "r");
    float X; float Y; float Z;
    for (int i =0; i < 6; i++) {
        fscanf(file, "%Lf %Lf %Lf", &X, &Y, &Z);
        for (int j =0; j < 3; j++) {
            variables->mat[3*i+j][4*j]=X;
            variables->mat[3*i+j][4*j+1]=Y;
            variables->mat[3*i+j][4*j+2]=Z;
            variables->mat[3*i+j][4*j+3]=1;
        }
    }
    fclose(file);
}


int main() {
    // Création des matrices nécessaires
    matrice* variables = matrice_nulle_pointeur(3 * 6, 3 * 6); // Matrice pour stocker les points
    matrice* P_col = matrice_nulle_pointeur(18, 1);             // Matrice pour stocker le résultat de la SVD

    // Vérification de l'initialisation des matrices
    print_matrice(*variables);
    print_matrice(*P_col);

    // Chargement des points à partir des fichiers
    load_points_images("points/points_img1.txt", variables, 0);
    load_points_reels("points/points_reel.txt", variables, 0);

    // Vérification du contenu de la matrice 'variables' après chargement
    print_matrice(*variables);

    // Calcul de la SVD pour trouver la solution non triviale
    mtxSVDAx0_print(variables, P_col);

    // Vérification du résultat de la SVD dans P_col
    // Construction de la matrice P (3x4) à partir des résultats de P_col
    matrice P = matrice_nulle(3, 4);
    P.mat[0][0] = P_col->mat[0][0];
    P.mat[0][1] = P_col->mat[1][0];
    P.mat[0][2] = P_col->mat[2][0];
    P.mat[0][3] = P_col->mat[3][0];
    
    P.mat[1][0] = P_col->mat[4][0];
    P.mat[1][1] = P_col->mat[5][0];
    P.mat[1][2] = P_col->mat[6][0];
    P.mat[1][3] = P_col->mat[7][0];
    
    P.mat[2][0] = P_col->mat[8][0];
    P.mat[2][1] = P_col->mat[9][0];
    P.mat[2][2] = P_col->mat[10][0];
    P.mat[2][3] = P_col->mat[11][0];

    // Affichage de la matrice P
    print_matrice(P);

    // Libération de la mémoire des matrices dynamiques
    free_matrice(*variables);
    free(variables);
    free_matrice(*P_col);
    free(P_col);
    return 0;
}
