#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"
#define DISTANCE_SEUIL 10
#define HAMMING_SEUIL 30
#define RANSAC_SEUIL 30

void changement_repere(matrice* points, int image_size){
    for (int i = 0; i < points->n; i++){
        points->mat[i][0]=image_size-1-(points->mat[i][0]);
    }
}

matrice* read_jpg(char* filename){
    char input_name[32];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
    if (!file_exists(input_name)){
        char command[128];
        snprintf(command, sizeof(command), "python3 jpg_to_txt2.py %s.jpg", filename);
        system(command);
    }
    printf("%s", input_name);
    matrice* input;
    read_matrice_from_file_dimension(&input, input_name);
    return input;
}

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

matrice* selection_moravec(char* filename, int* nbp, matrice* input){
    matrice* output=matrice_nulle(input->n,input->m);
    int nb_points=moravec(input, output);
    *nbp=nb_points;
    char output_name[128];
    char parametre[256];
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, SEUIL, WINDOW, PARAM);
    snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, SEUIL, WINDOW, PARAM);
    save_matrice_pbm(output, output_name, parametre);
    snprintf(output_name, sizeof(output_name), "points_%s.txt", filename);
    save_matrice_to_file(output, filename);
    printf("nb_points : %d\n",nb_points);
    matrice* points=bit_image_to_points(output, *nbp);
    return points;
}

/*Hypothèses : 
- srand initialisé
- img1 / retenus déclaré mais pas initialisé
- F connu */
void detection(char* filename1, char* filename2, matrice* img1, matrice* retenus, matrice* F){
    char export_char[32];
    int* nbp1=calloc(1,sizeof(int));
    int* nbp2=calloc(1,sizeof(int));
    matrice* input1=read_jpg(filename1);
    matrice* input2=read_jpg(filename2);
    matrice* img1_raw;
    matrice* img2;
    int** actif=NULL;
    matrice* output;
    //img1=selection_moravec(filename1,nbp1, input1);
    img2=selection_moravec(filename2,nbp2, input2);
    save_matrice_to_file_dimension(img1,"points_dodec0.txt");
    save_matrice_to_file_dimension(img2,"points_dodec1.txt");
    int nb_points=moravec_arr(input1,&actif);

    compute_score(img1_raw,actif,nb_points);
    filtre_mat(img1,actif,nb_points);

    *nbp1=img1->n;
    *nbp2=img2->n;
    //calcule des droites epipolaires pour chaque points de img1
    retenus=matrice_nulle(*nbp1,2);//coordonnées du point 
    uint128_t** retenus_descripteur=init_descriptor(*nbp1);//distance hamming
    int pairs[NUM_PAIRS][4];
    generer_paires(pairs);
    uint128_t** img1_descripteur=compute_brief(input1, img1, pairs);
    uint128_t** img2_descripteur=compute_brief(input2, img2, pairs);
    for (int i=0;i<(*nbp1);i++){
        matrice* X1=coo_vect(img1->mat[i][0], img1->mat[i][1]);
        matrice* l=epipolar_line(F,X1);
        int h_min=1000;//tres grand devant les valeurs prise par h
        for(int j=0;j<(*nbp2);j++){
            matrice* X2=coo_vect(img2->mat[j][0], img2->mat[j][1]);
            //pour chaque point retenu de img1, on prend les point de img2 à distance inferieur au seuil
            if (fabs(produit_scalaire(l,X2))<DISTANCE_SEUIL){
                //on compare les distances de hamming
                int h=hamming_distance(img2_descripteur[j],img1_descripteur[i]);
                if (h<h_min){
                    retenus->mat[i][0]=img2->mat[j][0];
                    retenus->mat[i][1]=img2->mat[j][1];
                    h_min=h;
                }
            }
        }
    }
    //Suppression des aberrations
    //on verifie que les distances de hamming sont suffisement proche, si oui, on valide la paire, sinon on l'invalide
    for (int i=0;i<(*nbp1);i++){
        if (hamming_distance(retenus_descripteur[i],img1_descripteur[i])<HAMMING_SEUIL){
            retenus->mat[i][0]=-1;
            img1->mat[i][0]=-1;
            retenus->mat[i][1]=-1;
            img1->mat[i][1]=-1;
        }
    }
    snprintf(export_char, 32, "points_ap_%s.txt", filename1);
    save_matrice_to_file(img1, export_char);
    snprintf(export_char, 32, "points_ap_%s.txt", filename2);
    save_matrice_to_file(retenus, export_char);
    free(actif);
    //print_matrice(*img1);
    return 0;
}

