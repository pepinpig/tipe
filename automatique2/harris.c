#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"

#define WINDOW 2  // Taille de la fenêtre pour les calculs
#define SEUIL 100 // Seuil pour la détection des coins
#define K 0.004    // Paramètre empirique pour le calcul de R

// Filtre de Sobel pour les dérivées partielles
int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

// Convolution
double convolve(matrice* image, int x, int y, int kernel[3][3]) {
    double sum = 0.0;
    for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
            int nx = x + kx;
            int ny = y + ky;
            if (nx >= 0 && nx < image->m && ny >= 0 && ny < image->n) {
                sum += image->mat[ny][nx] * kernel[ky + 1][kx + 1];
            }
        }
    }
    return sum;
}

// algorithme de Harris
void harris(matrice* image_input, matrice* image_output) {
    for (int y = 1; y < image_input->n - 1; y++) {
        for (int x = 1; x < image_input->m - 1; x++) {
            // Calcul des dérivées partielles
            double Ix = convolve(image_input, x, y, sobel_x);
            double Iy = convolve(image_input, x, y, sobel_y);
            
            // Calcul des termes de la matrice de Harris
            double Ixx = Ix * Ix;
            double Iyy = Iy * Iy;
            double Ixy = Ix * Iy;

            // Moyenne des valeurs dans la fenêtre (2x2 autour du pixel)
            double sumIxx = 0.0, sumIyy = 0.0, sumIxy = 0.0;
            for (int dy = -WINDOW; dy <= WINDOW; dy++) {
                for (int dx = -WINDOW; dx <= WINDOW; dx++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < image_input->m && ny >= 0 && ny < image_input->n) {
                        sumIxx += Ixx;
                        sumIyy += Iyy;
                        sumIxy += Ixy;
                    }
                }
            }

            // Calcul de la matrice de Harris
            double detA = sumIxx * sumIyy - sumIxy * sumIxy;
            double traceA = sumIxx + sumIyy;
            double R = detA - K * traceA * traceA;  // Calcul du score de Harris

            // Application du seuil
            if (R > SEUIL) {
                image_output->mat[y][x] = 1;  // Coin détecté
            } else {
                image_output->mat[y][x] = 0;  // Pas de coin
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_image>\n", argv[0]);
        return 1;
    }
    char* filename = argv[1];
    char input_name[32];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
    if (!file_exists(input_name)) {
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }
    matrice* input;
    printf("%s", input_name);
    read_matrice_from_file_dimension(&input, input_name);
    matrice* output = matrice_nulle_pointeur(input->n, input->m);
    harris(input, output);
    char output_name[128];
    char parametre[256];
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, k:%f", filename, SEUIL, WINDOW, K);
    snprintf(output_name, sizeof(output_name), "harris-%s-%d-%d-%f.pbm", filename, SEUIL, WINDOW, K);
    save_matrice_pbm(output, output_name, parametre);
    free_matrice(*input);
    free_matrice(*output);
    return 0;
}
