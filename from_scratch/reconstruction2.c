#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "SVD.h"
#define N 6

matrice centre_optique(matrice P) {
    // Vérification des dimensions de la matrice P
    if (P.n != 3 || P.m != 4) {
        fprintf(stderr, "Erreur : La matrice P doit être de taille 3x4.\n");
        exit(EXIT_FAILURE);
    }

    // Étape 1 : Extraire la matrice 3x3 des coefficients associés aux points homogènes
    matrice P3x3 = sous_matrice(P, 0, 0, 3, 3); // Extraction des 3 premières colonnes

    // Étape 2 : Calcul du noyau (kernel) de P3x3
    // Cela revient à résoudre le système linéaire P3x3 * X = -P3x4 (où X est le centre optique)
    matrice X = matrice_nulle(3, 1); // Solution (X, Y, Z)

    // Calcul de -P3x4 (la colonne 4 de P avec un signe opposé)
    matrice moins_P3x4 = colonne(P, 3);
    multiplication_scalaire(moins_P3x4, -1);

    // Résolution du système linéaire
    if (!resoudre_systeme(P3x3, moins_P3x4, &X)) {
        fprintf(stderr, "Erreur : Impossible de résoudre le système pour trouver le centre optique.\n");
        exit(EXIT_FAILURE);
    }

    // Libération des matrices temporaires
    liberer_matrice(P3x3);
    liberer_matrice(moins_P3x4);

    return X; // Retourne le centre optique sous forme d'un vecteur colonne (3x1)
}


matrice intersection_rayons(matrice K1, matrice K2, 
                            matrice P1, matrice P2, 
                            long double u1, long double v1, 
                            long double u2, long double v2) {
    // Étape 1 : Calcul des directions
    matrice point1 = matrice_nulle(3, 1);
    point1.mat[0][0] = u1;
    point1.mat[1][0] = v1;
    point1.mat[2][0] = 1.0;

    matrice point2 = matrice_nulle(3, 1);
    point2.mat[0][0] = u2;
    point2.mat[1][0] = v2;
    point2.mat[2][0] = 1.0;

    matrice d1 = produit(inverse(K1), point1);
    matrice d2 = produit(inverse(K2), point2);

    // Étape 2 : Extraction des centres optiques
    matrice C1 = centre_optique(P1);
    matrice C2 = centre_optique(P2);

    // Étape 3 : Construction du système pour t et s
    matrice moins_C1 = copie(C1);
    multiplication_scalaire(moins_C1, -1);
    matrice C2_C1 = somme(C2, moins_C1);

    long double a11 = produit_scalaire(d1, d1);  // produit_scalaire pour vecteurs
    long double a12 = -produit_scalaire(d1, d2);
    long double a22 = produit_scalaire(d2, d2);

    long double b1 = produit_scalaire(C2_C1, d1);
    long double b2 = produit_scalaire(C2_C1, d2);

    // Résolution du système
    long double det = a11 * a22 - a12 * a12;
    long double t = (b1 * a22 - b2 * a12) / det;
    long double s = (b2 * a11 - b1 * a12) / det;

    // Étape 4 : Calcul des points sur les droites
    matrice P1_t = copie(d1);
    multiplication_scalaire(P1_t, t);
    matrice P1_final = somme(C1, P1_t);

    matrice P2_s = copie(d2);
    multiplication_scalaire(P2_s, s);
    matrice P2_final = somme(C2, P2_s);

    // Étape 5 : Calcul de l'intersection moyenne
    matrice M = somme(P1_final, P2_final);
    multiplication_scalaire(M, 0.5);

    // Libération des matrices temporaires
    liberer_matrice(point1);
    liberer_matrice(point2);
    liberer_matrice(d1);
    liberer_matrice(d2);
    liberer_matrice(C1);
    liberer_matrice(C2);
    liberer_matrice(moins_C1);
    liberer_matrice(C2_C1);
    liberer_matrice(P1_t);
    liberer_matrice(P1_final);
    liberer_matrice(P2_s);
    liberer_matrice(P2_final);

    return M;
}


int main(int argc, char const *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2>\n", argv[0]);
        return 1;
    }

    // Allouer les tableaux pour les coordonnées des points
    long double* X1 = calloc(N, sizeof(long double));
    long double* Y1 = calloc(N, sizeof(long double));
    long double* Z1 = calloc(N, sizeof(long double));
    long double* u1 = calloc(N, sizeof(long double));
    long double* v1 = calloc(N, sizeof(long double));

    long double* X2 = calloc(N, sizeof(long double));
    long double* Y2 = calloc(N, sizeof(long double));
    long double* Z2 = calloc(N, sizeof(long double));
    long double* u2 = calloc(N, sizeof(long double));
    long double* v2 = calloc(N, sizeof(long double));

    // Récupérer les noms des images
    const char* image_name1 = argv[1];
    const char* image_name2 = argv[2];

    char points_image_file1[256];
    char points_reel_file1[256];
    snprintf(points_image_file1, sizeof(points_image_file1), "points/points_%s.txt", image_name1);
    snprintf(points_reel_file1, sizeof(points_reel_file1), "points/points_reel%s.txt", image_name1);

    char points_image_file2[256];
    char points_reel_file2[256];
    snprintf(points_image_file2, sizeof(points_image_file2), "points/points_%s.txt", image_name2);
    snprintf(points_reel_file2, sizeof(points_reel_file2), "points/points_reel%s.txt", image_name2);

    printf("Lecture des points ...\n");
    load_all_points_images(points_image_file1, u1, v1);
    load_all_points_reels(points_reel_file1, X1, Y1, Z1);
    load_all_points_images(points_image_file2, u2, v2);
    load_all_points_reels(points_reel_file2, X2, Y2, Z2);

    // Charger les matrices P1 et P2
    char p_file1[256], p_file2[256];
    snprintf(p_file1, sizeof(p_file1), "P-%s.txt", image_name1);
    snprintf(p_file2, sizeof(p_file2), "P-%s.txt", image_name2);
    printf("Lecture des matrices de projection ...\n");
    matrice P1 = lire_matrice(p_file1);
    matrice P2 = lire_matrice(p_file2);

    // Charger les matrices K1 et K2
    char k_file1[256], k_file2[256];
    snprintf(k_file1, sizeof(p_file1), "K-%s.txt", image_name1);
    snprintf(k_file2, sizeof(p_file2), "K-%s.txt", image_name2);
    printf("Lecture des matrices de intrisèques ...\n");
    matrice K1 = lire_matrice(k_file1);
    matrice K2 = lire_matrice(k_file2);

    // Préparer le fichier de sortie
    char output_file[256];
    snprintf(output_file, sizeof(output_file), "points/points_reconstruit.txt");
    FILE* output = fopen(output_file, "w");
    if (!output) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        return 1;
    }

    printf("Reconstruction des points 3D ...\n");

    fclose(output);
    printf("Points reconstruits écrits dans : %s\n", output_file);

    // Libération des ressources
    free(X1); free(Y1); free(Z1); free(u1); free(v1);
    free(X2); free(Y2); free(Z2); free(u2); free(v2);
    free_matrice(P1);
    free_matrice(P2);

    return 0;
}


