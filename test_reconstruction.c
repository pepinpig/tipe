#include "reconstruction.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2>\n", argv[0]);
        return 1;
    }
    char* image_name1 = argv[1];
    char* image_name2 = argv[2];

    matrice* matrice_output;
    int nb_points =reconstruction1(image_name1, image_name2, &matrice_output);
    char command[256];
    snprintf(command, sizeof(command), "python3 plot_points_3D.py points_3d_%s", image_name1);
    system(command);
    return 0;
}