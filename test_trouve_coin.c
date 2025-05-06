#include"trouve_coin.h"

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
    matrice* output;
    printf("%s", input_name);
    read_matrice_from_file_dimension(&input, input_name);
    int** actif=NULL;
    int nb_points=moravec_arr(input,&actif);
    printf("nb_points : %d",nb_points);
    output=compute_score(input,actif,nb_points);
    filtre_mat(output,actif,nb_points);
    char output_name[128];
    char parametre[256];
    printf("test\n");
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, SEUIL, WINDOW, PARAM);
    snprintf(output_name, sizeof(output_name), "tc-%s-mv-%d-%d-%d.pbm", filename, SEUIL, WINDOW, PARAM);
    save_matrice_pbm(output, output_name, parametre);
    save_matrice_to_file(output, filename);
    free_matrice(input);
    free_matrice(output);
    for(int i= 0;i<nb_points;i++){
      free(actif[i]);
    }
    free(actif);
    return 0;
}
