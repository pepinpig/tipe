#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"

// Fonction pour sélectionner la meilleure correspondance entre deux images
int select_best_match(int n1, int n2, float F[3][3]);


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

long double distance_point_droite(matrice* l,matrice* X) {
    long double a=l->mat[0][0];
    long double b=l->mat[1][0];
    long double c=l->mat[2][0];
    long double x=X->mat[0][0];
    long double y=X->mat[1][0];
    return fabs(a * x + b * y + c) / sqrt(a * b + b * b);
}

matrice* slection_moravec(char* filename, nbp){
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
    nbp=&nb_points;
    char output_name[128];
    char parametre[256];
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, SEUIL, WINDOW, PARAM);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, SEUIL, WINDOW, PARAM);
    //save_matrice_pbm(output, output_name, parametre);
    //save_matrice_to_file(output, filename);
    printf("nb_points : %d",nb_points);
    free_matrice(*input);
    return output;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2>\n", argv[0]);
        return 1;
    }
    //detection des points d'interet avec Moravec pour il
    char* filename1=argv[1];
    char* filename2=argv[2];
    int* nbp1;
    int* nbp2;
    matrice* img1=selection_moravec(filename1,nbp1);
    matrice* img2=selection_moravec(filename2,nbp2);
    //tri de points utiles sur image 1
    //....
    //calcule des droites epipolaires pour chaque points de img1
    matrice* distance=matrice_nulle_pointeur(nbp2,2);
    matrice* retenus=matrice_nulle_pointeur(nbp1,3); //coordonnées du point + distance hamming
    matrice* F=compute_F(filename1, filename2);
    matrice* img1_descripteur=compute_brief()
    matrice* img2_descripteur=compute_brief()
    for (int i=0;i<*nbp1;i++){
        matrice* X1=coo_vect(img1->mat[i][0], img1->mat[i][1]);
        matrice* l=epipolar_line(F,X1);
        for(int j=0;j<*nbp2;j++){
            matrice* X2=coo_vect(img2->mat[i][0], img2->mat[i][1]);
                //pour chaque point retenu de img1, on prend les point de img2 à distance inferieur au seuil
            if (distance_point_droite(l,X2)<SEUIL){
                //on compare les distances de hamming
                int h=hamming_distance(img2_descripteur->mat[j][0],img1_descripteur->mat[i][0], img1->mat[i][1]);
                if (h<retenus->mat[i][2]){
                    retenus->mat[i][0]=img2->mat[j][0];
                    retenus->mat[i][1]=img2->mat[j][1];
                    retenus->mat[i][2]=h;
                }
            }
        }
    }
    //on verifie que les distances de hamming sont suffisement proche, si oui, on valide la paire, sinon on l'invalide
    for (int i=0;i<*nbp1;i++){
        if (hamming_distance(retenus->mat[i][0],img1->mat[i][0])<SEUIL2){
            retenus->mat[i][0]=NULL;
            img1->mat[i][0]=NULL;
            retenus->mat[i][1]=NULL;
            img1->mat[i][1]=NULL;
        }
    }
    return 0;
}
