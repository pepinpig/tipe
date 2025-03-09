#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "detection.h"

#pragma region Moravec

// Calcul de la variance pour une direction donnée avec des paramètres pour la fenêtre et le seuil
double variance(matrice* image, int x, int y, int dx, int dy, int window) {
    int count = 0; int sum = 0; int sumSq = 0;
    for (int i = -window; i <= window; ++i) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < image->m && ny >= 0 && ny < image->n) {
            int val = image->mat[ny][nx];
            sum += val;
            sumSq += val * val;
            count++;
        }
    }
    return (count > 0) ? (sumSq - (sum * sum) / count) : 0;
}

// Application de l'algorithme de Moravec pour la détection des coins avec des paramètres
int moravec(matrice* image_input, matrice* image_output, int window, int seuil, int param) {
    int count=0;
    for (int y = 1; y < image_input->n - 1; y++) {
        for (int x = 1; x < image_input->m - 1; x++) {
            int var0 = variance(image_input, x, y, 0, 1, window);  // Verticale
            int var1 = variance(image_input, x, y, 1, 0, window);  // Horizontale
            int var2 = variance(image_input, x, y, 1, 1, window);  // Diagonale bas gauche haut droite
            int var3 = variance(image_input, x, y, 1, -1, window); // Diagonale haut gauche bas droite

            int vartot = var0;  // Commence par la première direction
            if (param){
                if (var1 < vartot) vartot = var1;
                if (var2 < vartot) vartot = var2;
                if (var3 < vartot) vartot = var3;
            }
            else{
                vartot += var1;
                vartot += var2;
                vartot += var3;
                vartot /= 4;
            }
            if (vartot > seuil) {
                image_output->mat[y][x] = 1;  // Coin détecté
                count++;
            } else {
                image_output->mat[y][x] = 0;  // Pas de coin
            }
        }
    }
    return count;
}

void complete_output_points(matrice* output, matrice* output_points, int nb_points){
    int c=0;
    for (int i = 0; i < output->n; i++){
        for (int j = 0; j < output->m; j++){
            if(output->mat[i][j]==1){
                output_points->mat[c][0]=i;
                output_points->mat[c][1]=j;
                c++;
            }
        }
    }
}

// Rempli la matrice output avec une copie bitmap de l'image et retourne une matrice
// contenant la liste des points retenus stockées dans une matrice de taille nb_points*4 (2 dernières colonnes vides)
matrice* compute_moravec(matrice* input, matrice* output, char* filename, int* nb_points, int window, int seuil, int param) {
    *nb_points = moravec(input, output, window, seuil, param);
    matrice* output_points = matrice_nulle_pointeur(*nb_points, 4); // Les deux dernières colonnes vont accueillir le descripteur
    complete_output_points(output, output_points, *nb_points);
    printf("nb_points : %d \n", *nb_points);
    return output_points;
}

#pragma endregion

#pragma region Brief
#define PATCH_SIZE 9
#define NUM_BITS 256
#define NUM_PAIRS (NUM_BITS / 2)  // 128 paires pour remplir deux colonnes

// Générer des paires de tests BRIEF avec des paramètres pour la taille de patch et le nombre de bits
void generer_paires(int pairs[NUM_PAIRS][4], int patch_size) {
    for (int i = 0; i < NUM_PAIRS; i++) {
        pairs[i][0] = rand() % patch_size - patch_size / 2; // x1
        pairs[i][1] = rand() % patch_size - patch_size / 2; // y1
        pairs[i][2] = rand() % patch_size - patch_size / 2; // x2
        pairs[i][3] = rand() % patch_size - patch_size / 2; // y2
    }
}

// Récupérer l'intensité d'un pixel dans l'image (grayscale 2D array)
uint8_t get_pixel(matrice* image, int x, int y) {
    if (x < 0 || x >= image->m || y < 0 || y >= image->n) return 0;
    return (uint8_t) image->mat[y][x];
}

// Générer le descripteur BRIEF et l'ajouter à la matrice "points"
void compute_brief(matrice* image, matrice* points, int patch_size, int num_bits) {
    int pairs[NUM_PAIRS][4];
    generer_paires(pairs, patch_size);  // Générer les paires de test
    
    for (int i = 0; i < points->n; i++) {
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];

        uint64_t descriptor1 = 0;
        uint64_t descriptor2 = 0;

        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];

            uint8_t p1 = get_pixel(image, x1, y1);
            uint8_t p2 = get_pixel(image, x2, y2);

            if (j < 64) {
                if (p1 < p2) descriptor1 |= (1ULL << j);
            } else {
                if (p1 < p2) descriptor2 |= (1ULL << (j - 64));
            }
        }

        // Stocker les deux parties du descripteur dans les colonnes 2 et 3
        points->mat[i][2] = (long double) descriptor1;
        points->mat[i][3] = (long double) descriptor2;
    }
}

#pragma endregion
#pragma region distance epipolaire



#pragma end region
#pragma region Main

matrice* detection_aux(char* filename, int window, int seuil, int param, matrice* output) {
    matrice* input;
    int* nb_points = malloc(sizeof(int)); 
    char input_name[64];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);

    if (!file_exists(input_name)) {
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }

    read_matrice_from_file_dimension(&input, input_name);
    printf("%s\n", input_name);

    matrice* points = compute_moravec(input, output, filename, nb_points, window, seuil, param);
    compute_brief(input, points, PATCH_SIZE, NUM_BITS); 
    free_matrice(*input);  
    return points;
}

matrice* detection(char* filename, int window, int seuil, int param) {
    matrice* output = matrice_nulle_pointeur(0, 0);
    matrice* points = detection_aux(filename, window, seuil, param, output);
    return points;
}

matrice* detection_impression(char* filename, int window, int seuil, int param) {
    matrice* input;
    read_matrice_from_file_dimension(&input, filename); 
    matrice* output = matrice_nulle_pointeur(input->n, input->m);
    matrice* points = detection_aux(filename, window, seuil, param, output);
    char output_name_points[128];
    char output_name[128];
    char parametre[256];

    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, seuil, window, param);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, seuil, window, param);
    snprintf(output_name_points, sizeof(output_name_points), "%s-mv-points.txt", filename);

    save_matrice_pbm(output, output_name, parametre);
    save_matrice_to_file(output, filename);
    save_matrice_to_file_dimension(points, output_name_points);
    print_matrice(*points);

    free_matrice(*input); 
    free_matrice(*output);  

    return points; 
}

int main(){
    detection(dodec, 1000,2, 1);
    return 0;
}
#pragma endregion



#pragma end region
#pragma region Main

matrice* detection_aux(char* filename, int window, int seuil, int param, matrice* output) {
    matrice* input;
    int* nb_points = malloc(sizeof(int)); 
    char input_name[64];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);

    if (!file_exists(input_name)) {
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }

    read_matrice_from_file_dimension(&input, input_name);
    printf("%s\n", input_name);

    matrice* points = compute_moravec(input, output, filename, nb_points, window, seuil, param);
    compute_brief(input, points, PATCH_SIZE, NUM_BITS); 
    free_matrice(*input);  
    return points;
}

matrice* detection(char* filename, int window, int seuil, int param) {
    matrice* output = matrice_nulle_pointeur(0, 0);
    matrice* points = detection_aux(filename, window, seuil, param, output);
    return points;
}

matrice* detection_impression(char* filename, int window, int seuil, int param) {
    matrice* input;
    read_matrice_from_file_dimension(&input, filename); 
    matrice* output = matrice_nulle_pointeur(input->n, input->m);
    matrice* points = detection_aux(filename, window, seuil, param, output);
    char output_name_points[128];
    char output_name[128];
    char parametre[256];

    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, seuil, window, param);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, seuil, window, param);
    snprintf(output_name_points, sizeof(output_name_points), "%s-mv-points.txt", filename);

    save_matrice_pbm(output, output_name, parametre);
    save_matrice_to_file(output, filename);
    save_matrice_to_file_dimension(points, output_name_points);
    print_matrice(*points);

    free_matrice(*input); 
    free_matrice(*output);  

    return points; 
}

int main(){
    //detection des points d'interet avec Moravec pour il
    img1;
    img2;
    points1 = detection(img1, 1000,2, 1);
    points2 = detection(img2, 1000,2,1);
    //tri de points utiles sur image 1
    //calcule des droites epipolaires pour chaque points de img1
    //pour chaque point retenu de img1, on prend les point de img2 à distance inferieur au seuil
    //on compare les distances de hamming
    //on verifie que les distances de hamming sont suffisement proche
    return 0;
}
#pragma endregion
