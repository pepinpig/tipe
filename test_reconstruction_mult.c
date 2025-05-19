#include "reconstruction.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image>*8\n", argv[0]);
        return 1;
    }
    char* image_name1 = argv[1];
    char* image_name2 = argv[2];
    char* image_name3 = argv[3];
    char* image_name4 = argv[4];
    char* image_name5 = argv[5];
    char* image_name6 = argv[6];
    char* image_name7 = argv[7];
    char* image_name8 = argv[8];

    matrice* matrice_output;
    int nb_points =reconstruction4(image_name1, image_name2,image_name3, image_name4,image_name5, image_name6,image_name7, image_name8, &matrice_output);
    char export[128];
    snprintf(export, sizeof(export), "points_3d_%s_all.txt", image_name1);
    save_matrice_to_file(matrice_output,export );
    char command[256];
    snprintf(command, sizeof(command), "python3 plot_points_3D.py points_3d_%s points_3d_%s points_3d_%s points_3d_%s", image_name1, image_name3, image_name5, image_name7);
    system(command);
    return 0;
}
