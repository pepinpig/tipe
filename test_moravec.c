#include"moravec.h"

matrice* bit_image_to_points (matrice* image, int nb_points){
    matrice* res=matrice_nulle(nb_points,2);
    int c=0;
    for (int i = 0; i < image->n; i++){
        for (int j=0; j<image->m; j++){
            if ((image->mat[i][j])==1){
                res->mat[c][0]=i;
                res->mat[c][1]=j;
                c++;
            }
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_image>\n", argv[0]);
        return 1;
    }
    char* filename=argv[1];
    char input_name[32];
    char image_name[32];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
    snprintf(image_name, sizeof(input_name), "%s.jpg", filename);
    if (!file_exists(input_name)){
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }
    matrice* input;
    printf("%s", input_name);
    read_matrice_from_file_dimension(&input, input_name);
    matrice* output=matrice_nulle(input->n,input->m);
    printf("%d, %d\n", output->n, output->m);
    int nb_points=moravec(input, output);
    char output_name[128];
    char parametre[256];
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, Seuil_moravec, Window, PARAM);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, Seuil_moravec, Window, PARAM);
    save_matrice_pbm(output, output_name, parametre);
    save_matrice_to_file(output, filename);
    matrice* points=bit_image_to_points(output, nb_points);
    char command[500];
    char export_char[32];
    snprintf(export_char, 32, "points_ap_%s.txt",filename);
    save_matrice_to_file(points, export_char);
    snprintf(command, sizeof(command), "python3 plot_detect_un.py %s %s",image_name, export_char);
    //system(command);
    printf("nb_points : %d",nb_points);
    free_matrice(input);
    free_matrice(output);
    return 0;
}
