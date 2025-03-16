#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "detection.h"


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

double distance_point_droite(double x, double y, double a, double b, double c) {
    return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
}

int main(){
    //detection des points d'interet avec Moravec pour il


    //tri de points utiles sur image 1
    //calcule des droites epipolaires pour chaque points de img1
    //pour chaque point retenu de img1, on prend les point de img2 à distance inferieur au seuil
    //on compare les distances de hamming
    //on verifie que les distances de hamming sont suffisement proche
    return 0;
}
#pragma endregion
