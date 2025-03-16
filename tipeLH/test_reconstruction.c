#include "camera_calibration9.h"

int main (int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2> <couleurs>\n", argv[0]);
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
    char* image_name1 = argv[1];
    char* image_name2 = argv[2];
    char* cl =argv[3];

    char points_image_file1[256];
    char points_reel_file1[256];
    snprintf(points_image_file1, sizeof(points_image_file1), "points/donnees/points_%s.txt", image_name1);
    snprintf(points_reel_file1, sizeof(points_reel_file1), "points/donnees/points_reels_%s.txt", cl);

    char points_image_file2[256];
    char points_reel_file2[256];
    snprintf(points_image_file2, sizeof(points_image_file2), "points/donnees/points_%s.txt", image_name2);
    snprintf(points_reel_file2, sizeof(points_reel_file2), "points/donnees/points_reels_%s.txt", cl);

    printf("Lecture des points ...\n");
    load_all_points_images(points_image_file1, u1, v1);
    load_all_points_reels(points_reel_file1, X1, Y1, Z1);
    load_all_points_images(points_image_file2, u2, v2);
    load_all_points_reels(points_reel_file2, X2, Y2, Z2);

    // Charger les matrices P1 et P2
    char p_file1[256], p_file2[256];
    matrice* P1=matrice_nulle_pointeur(3,4);
    matrice* P2=matrice_nulle_pointeur(3,4);
    nom_fichier(p_file1, "P", image_name1);
    nom_fichier(p_file2, "P", image_name2);
    read_matrice_from_file(P1, p_file1);
    read_matrice_from_file(P2, p_file2);
    // Préparer le fichier de sortie
    char output_file[256];
    nom_fichier(output_file,"points/donnees/points_reconstruits",image_name1);
    FILE* output = fopen(output_file, "w");
    assert(output!=NULL);
    printf("Reconstruction des points 3D ...\n");
    for (int i = 0; i < N; ++i) {
        // Construire les matrices pour le système d'équations
        matrice A = matrice_nulle(4, 4);
        // Remplir la matrice A avec les équations pour les points des deux images
        for (int j = 0; j < 4; ++j) {
            A.mat[0][j] = u1[i] * P1->mat[2][j] - P1->mat[0][j];
            A.mat[1][j] = v1[i] * P1->mat[2][j] - P1->mat[1][j];
            A.mat[2][j] = u2[i] * P2->mat[2][j] - P2->mat[0][j];
            A.mat[3][j] = v2[i] * P2->mat[2][j] - P2->mat[1][j];
        }
        // Résolution par SVD pour obtenir les coordonnées 3D

        // Étape 1 : Calcul de la décomposition SVD de A
        matrice U, S, V; // Matrices pour la décomposition SVD
        qr_algorithm_SVD(&A, &U, &S, &V);
        // Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
        long double min_sigma = S.mat[S.n - 1][S.m - 1];
        int index_min = S.n - 1;
        matrice p = matrice_nulle(V.m, 1);
        for (int i = 0; i < V.m; i++) {
            p.mat[i][0] = V.mat[i][index_min];
        }
        //Normalisation homogène
        for (int i = 0; i < 3; ++i){
            p.mat[i][0]/=p.mat[3][0];
        }
        fprintf(output, "%Lf %Lf %Lf\n",  p.mat[0][0], p.mat[1][0], p.mat[2][0]);
    }

    fclose(output);
    printf("Points reconstruits écrits dans : %s\n", output_file);

    // Libération des ressources
    free(X1); free(Y1); free(Z1); free(u1); free(v1);
    free(X2); free(Y2); free(Z2); free(u2); free(v2);
/*    free_matrice(P1);
    free_matrice(P2);*/

    return 0;
}