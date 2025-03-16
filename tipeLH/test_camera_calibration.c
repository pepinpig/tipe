#include "camera_calibration9.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_de_l'image> <couleurs> \n", argv[0]);
        return 1;
    }
    long double* X = calloc(N, sizeof(long double));
    long double* Y = calloc(N, sizeof(long double));
    long double* Z = calloc(N, sizeof(long double));
    long double* u = calloc(N, sizeof(long double));
    long double* v = calloc(N, sizeof(long double));
    char* cl =argv[2];
    char* image_name =argv[1];
    char points_image_file[256];
    char points_reel_file[256];
    snprintf(points_image_file, sizeof(points_image_file), "points/donnees/points_calibrage_%s.txt", image_name);
    snprintf(points_reel_file, sizeof(points_reel_file), "points/donnees/points_reels_%s.txt", cl);
    load_all_points_images(points_image_file, u, v); 
    printf("test\n");
    load_all_points_reels(points_reel_file, X, Y, Z);
    matrice* A = contruction_A(X, Y, Z, u, v);
    char fn[100];
    nom_fichier(fn, "A", image_name);
    save_matrice_to_file(A, fn);
    matrice P = matrice_nulle(3, 4);
    camera_calibration_resolution(&P, A);
    nom_fichier(fn, "P", image_name);
    save_matrice_to_file(&P, fn);

    free(X);
    free(Y);
    free(Z);
    free(u);
    free(v);
    free_matrice(*A);
    return 0;
}
