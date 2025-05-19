#include "camera_calibration.h"

void calibration_un(char* image_name, matrice* P, matrice* K, matrice* R, matrice* T, double* X, double* Y, double* Z){
    char points_image_file[256];
    double* u = calloc(N, sizeof(double));
    double* v = calloc(N, sizeof(double));
    snprintf(points_image_file, sizeof(points_image_file), "points/donnees/points_calibrage_%s.txt", image_name);
    load_all_points_images(points_image_file, u, v, N); 
    matrice* A = construction_A(X, Y, Z, u, v, N);
    char fn[100];
    nom_fichier(fn, "A", image_name);
    save_matrice_to_file(A, fn);
    camera_calibration_resolution(P, A, K, R, T);
    nom_fichier(fn, "P", image_name);
    save_matrice_to_file(P, fn);
    free(u);
    free(v);
    free_matrice(A);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_de_l'image1> <nom_de_l'image2> <couleurs> [select]\n", argv[0]);
        return 1;
    }
    double* X = calloc(N, sizeof(double));
    double* Y = calloc(N, sizeof(double));
    double* Z = calloc(N, sizeof(double));
    char* cl =argv[3];
    char* image_name1 =argv[1];
    char* image_name2 =argv[2];
    char points_reel_file[256];
    char export_char[32];
    char command[128];
    if(argc>4){
        snprintf(command, sizeof(command), "python3 select_deux.py %s.jpg %s.jpg", image_name1, image_name2);
        system(command);
    }
    else{
        printf("lecture des fichiers...\n");
    }
    snprintf(points_reel_file, sizeof(points_reel_file), "points/donnees/points_reels_%s.txt", cl);
    load_all_points_reels(points_reel_file, X, Y, Z, N);
    matrice* P1 = matrice_nulle(3, 4); matrice* P2 = matrice_nulle(3, 4);
    matrice* K1 = matrice_nulle(3, 3); matrice* K2 = matrice_nulle(3, 3);
    matrice* R1 = matrice_nulle(3, 3); matrice* R2 = matrice_nulle(3, 3);
    matrice* T1 = matrice_nulle(3, 1); matrice* T2 = matrice_nulle(3, 1);
    calibration_un(image_name1, P1, K1, R1, T1, X, Y, Z);
    calibration_un(image_name2, P2, K2, R2, T2, X, Y, Z);
    matrice* F= compute_F(K1 ,R1, T1, K2, R2, T2);
    snprintf(export_char, sizeof(export_char),"F_%s", image_name1);
    save_matrice_to_file(F, export_char);
    free(X);
    free(Y);
    free(Z);
    return 0;
}
