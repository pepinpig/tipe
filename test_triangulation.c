#include "triangle.h"
#include "reconstruction.h"

#define NBRPOINTS 50

int main(int argc, char* argv[]) {
    if (argc < 7) {
        fprintf(stderr, "Usage: %s <nom_image>*6\n", argv[0]);
        return 1;
    }
    char* image_name1 = argv[1];
    char* image_name2 = argv[2];
    char* image_name3 = argv[3];
    char* image_name4 = argv[4];
    char* image_name5 = argv[5];
    char* image_name6 = argv[6];
    matrice* matrice_output;
    int nb_points =reconstruction3(image_name1, image_name2,  image_name3,  image_name4,  image_name5,  image_name6, &matrice_output);
    double** env = mat_to_table (matrice_output,&nb_points);
    unsigned long int n = trois_parmi(nb_points);
    printf("malloc triangle debut\n");
    fflush(stdout);
    triangle* triangle_table = triangles(nb_points);
    printf("malloc triangle fin\n");
    fflush(stdout);
    //char fn_complete[512];
    //snprintf(fn_complete, sizeof(fn_complete), "points/donnees/tri_%s.txt", filename);
    //FILE* file2 = fopen(fn_complete, "w");

    bool* garde = keeptrig(triangle_table,n,nb_points,env);
    /*for (int i = 0; i < n; i++) {
        if (garde[i]) {
            for (int j = 0; j < 3; j++) {
                double* point = env[triangle_table[i][j]];
                file_print_vect(point, file2);
            }
            fprintf(file2, "---\n");
        }
    }*/
    stl_generate("test.stl",env,triangle_table,n,garde);
    //fclose(file2);
    free(garde);
    destroy_trigs(triangle_table);
    destroy_points(env, nb_points);

    return EXIT_SUCCESS;
}
