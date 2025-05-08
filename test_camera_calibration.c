#include "camera_calibration.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_de_l'image> <couleurs> (selection?)\n", argv[0]);
        return 1;
    }
    double* X = calloc(N, sizeof(double));
    double* Y = calloc(N, sizeof(double));
    double* Z = calloc(N, sizeof(double));
    double* u = calloc(N, sizeof(double));
    double* v = calloc(N, sizeof(double));
    char* cl =argv[2];
    char* image_name =argv[1];
    char points_image_file[256];
    char points_reel_file[256];
    char command[128];
    int n=N;
    if(argc>3){
        snprintf(command, sizeof(command), "python3 camera_calibration_save_points.py %s.jpg", image_name);
        system(command);
    }
    else{
        printf("lecture des fichiers...\n");
    }
    snprintf(points_image_file, sizeof(points_image_file), "points/donnees/points_calibrage_%s.txt", image_name);
    snprintf(points_reel_file, sizeof(points_reel_file), "points/donnees/points_reels_%s.txt", cl);
    load_all_points_images(points_image_file, u, v, n); 
    load_all_points_reels(points_reel_file, X, Y, Z, n);
    matrice* A = construction_A(X, Y, Z, u, v, n);
    char fn[100];
    nom_fichier(fn, "A", image_name);
    save_matrice_to_file(A, fn);
    matrice* P = matrice_nulle(3, 4);
    matrice* K = matrice_nulle(3, 3);
    matrice* R = matrice_nulle(3, 3);
    matrice* T = matrice_nulle(3, 1);
    camera_calibration_resolution(P, A, K, R, T);
    nom_fichier(fn, "P", image_name);
    save_matrice_to_file(P, fn);
    free(X);
    free(Y);
    free(Z);
    free(u);
    free(v);
    free_matrice(A);
    return 0;
}
