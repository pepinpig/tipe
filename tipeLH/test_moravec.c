#include"moravec.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_image>\n", argv[0]);
        return 1;
    }
    char* filename=argv[1];
    char input_name[32];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
    if (!file_exists(input_name)){
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }
    matrice* input;
    printf("%s", input_name);
    read_matrice_from_file_dimension(&input, input_name);
    matrice* output=matrice_nulle_pointeur(input->n,input->m);
    int nb_points=moravec(input, output);
    char output_name[128];
    char parametre[256];
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, SEUIL, WINDOW, PARAM);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, SEUIL, WINDOW, PARAM);
    save_matrice_pbm(output, output_name, parametre);
    save_matrice_to_file(output, filename);
    printf("nb_points : %d",nb_points);
    free_matrice(*input);
    free_matrice(*output);
    return 0;
}
