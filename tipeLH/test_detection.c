#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"
#define DISTANCE_SEUIL 100
#define HAMMING_SEUIL 100

long double distance_point_droite(matrice* l,matrice* X) {
    long double a=l->mat[0][0];
    long double b=l->mat[1][0];
    long double c=l->mat[2][0];
    long double x=X->mat[0][0];
    long double y=X->mat[1][0];
    return fabs(a * x + b * y + c) / sqrt(a * b + b * b);
}

matrice* read_jpg(char* filename){
    char input_name[32];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
    if (!file_exists(input_name)){
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt.py %s.jpg", filename);
        system(command);
    }
    printf("%s", input_name);
    matrice* input;
    read_matrice_from_file_dimension(&input, input_name);
    return input;
}

matrice* selection_moravec(char* filename, int* nbp, matrice* input){
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
    int* nbp1=calloc(1,sizeof(int));
    int* nbp2=calloc(1,sizeof(int));
    matrice* input1=read_jpg(filename1);
    matrice* input2=read_jpg(filename2);
    matrice* img1=selection_moravec(filename1,nbp1, input1);
    matrice* img2=selection_moravec(filename2,nbp2, input2);
    //tri de points utiles sur image 1
    //....
    //calcule des droites epipolaires pour chaque points de img1
    matrice* retenus=matrice_nulle_pointeur(*nbp1,1);//coordonnées du point 
    uint128_t* retenus_descripteur=calloc(*nbp1,sizeof(uint128_t));//distance hamming
    matrice* F=matrice_nulle_pointeur(3,3);
    read_matrice_from_file(F, "F.txt");
    srand(time(NULL));
    int pairs[NUM_PAIRS][4];
    generer_paires(pairs);
    uint128_t* img1_descripteur=compute_brief(input1, img1, pairs);
    uint128_t* img2_descripteur=compute_brief(input2, img2, pairs);
    for (int i=0;i<(*nbp1);i++){
        matrice* X1=coo_vect(img1->mat[i][0], img1->mat[i][1]);
        matrice* l=epipolar_line(F,X1);
        for(int j=0;j<*nbp2;j++){
            matrice* X2=coo_vect(img2->mat[i][0], img2->mat[i][1]);
                //pour chaque point retenu de img1, on prend les point de img2 à distance inferieur au seuil
            if (distance_point_droite(l,X2)<DISTANCE_SEUIL){
                //on compare les distances de hamming
                int h=hamming_distance(img2_descripteur[j],img1_descripteur[i]);
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
        if (hamming_distance(retenus_descripteur[i],img1_descripteur[i])<HAMMING_SEUIL){
            retenus->mat[i][0]=-1;
            img1->mat[i][0]=-1;
            retenus->mat[i][1]=-1;
            img1->mat[i][1]=-1;
        }
    }
    return 0;
}
