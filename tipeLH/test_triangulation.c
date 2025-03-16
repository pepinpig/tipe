#include "triangle.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
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
    int n = trois_parmi(point_count);
    int** triangle_table = triangles(point_count);
    char fn_complete[512];
    snprintf(fn_complete, sizeof(fn_complete), "points/donnees/tri_%s.txt", filename);
    FILE* file2 = fopen(fn_complete, "w");
    bool* garde = keeptrig(triangle_table,n,point_count,env);
    /*bool* garde=malloc(n*sizeof(bool));*/
    for (int i = 0; i < n; i++) {
        /*garde[i]=true;*/
        if (garde[i]) {
            for (int j = 0; j < 3; j++) {
                double* point = env[triangle_table[i][j]];
                file_print_vect(point, file2);
            }
            fprintf(file2, "---\n");
        }
    }
    fclose(file2);
    free(garde);
    destroy_trigs(triangle_table, n);
    destroy_points(env, point_count);

    return EXIT_SUCCESS;
}
