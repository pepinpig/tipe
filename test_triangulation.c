#include "triangle.h"

#define NBRPOINTS 50

int main(int argc, char* argv[]) {
  fprintf(stdout, "N = %d\n", NBRPOINTS); fflush(stdout);
    /*if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_fichier>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char* base_nom = argv[1];
    char filename[256];
    snprintf(filename,sizeof(filename), "points_3D-%s",base_nom);
    char fn_complete_n[256];
    snprintf(fn_complete_n, sizeof(fn_complete_n), "points/donnees/nb_points-%s.txt", base_nom);
    FILE* file = fopen(fn_complete_n, "r");
    if (!file) {
        printf("%s\n",fn_complete_n );
        perror("Erreur d'ouverture du fichier nb_points.txt");
        return EXIT_FAILURE;
    }
    int point_count;
    fscanf(file, "%d", &point_count);
    fclose(file);
    double** env = read_points(filename, point_count);
    */
    
    double** env = rand_points(NBRPOINTS);
    unsigned long int n = trois_parmi(NBRPOINTS);
    printf("malloc triangle debut\n");
    fflush(stdout);
    triangle* triangle_table = triangles(NBRPOINTS);
    printf("malloc triangle fin\n");
    fflush(stdout);
    //char fn_complete[512];
    //snprintf(fn_complete, sizeof(fn_complete), "points/donnees/tri_%s.txt", filename);
    //FILE* file2 = fopen(fn_complete, "w");

    bool* garde = keeptrig(triangle_table,n,NBRPOINTS,env);
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
    destroy_points(env, NBRPOINTS);

    return EXIT_SUCCESS;
}
