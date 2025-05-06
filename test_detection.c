#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"
#define DISTANCE_SEUIL 13
#define HAMMING_SEUIL 100

#define MAX_FILENAME 256


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


matrice* k(matrice* pt, matrice** P){
    matrice* XA=coo_vect(pt->mat[0][0], pt->mat[0][1]);
    matrice* XB=coo_vect(pt->mat[1][0], pt->mat[1][1]);
    matrice* XC=coo_vect(pt->mat[2][0], pt->mat[2][1]);
    matrice* XD=coo_vect(pt->mat[3][0], pt->mat[3][1]);
    *P=concatenation3(XA,XB,XC);
    matrice* k=resolution_systeme(*P,XD);
    return k;
}

matrice* H_pp(char* filename1, char* filename2){
    char export_char[32];
    matrice* pt1=matrice_nulle(6,2);
    snprintf(export_char, 32, "%s_c.txt", filename1);
    read_matrice_from_file(pt1, export_char);
    matrice* pt2=matrice_nulle(6,2);
    snprintf(export_char, 32, "%s_c.txt", filename2);
    read_matrice_from_file(pt2, export_char);
    matrice** P1=malloc(sizeof(matrice*));
    matrice** P2=malloc(sizeof(matrice*));;
    matrice* k1=k(pt1, P1);
    matrice* k2=k(pt2, P2);
    matrice* R=matrice_nulle(3, 1);
    R->mat[0][0]=k2->mat[0][0]/k1->mat[0][0];
    R->mat[1][0]=k2->mat[1][0]/k1->mat[1][0];
    R->mat[2][0]=k2->mat[2][0]/k1->mat[2][0];
    matrice* X2=matrice_nulle(3, 1);
    X2->mat[0][0]=(R->mat[0][0])*((*P2)->mat[0][0]);
    X2->mat[1][0]=(R->mat[1][0])*((*P2)->mat[0][1]);
    X2->mat[2][0]=(R->mat[2][0])*((*P2)->mat[0][2]);
    matrice* Y2=matrice_nulle(3, 1);
    Y2->mat[0][0]=(R->mat[0][0])*((*P2)->mat[1][0]);
    Y2->mat[1][0]=(R->mat[1][0])*((*P2)->mat[1][1]);
    Y2->mat[2][0]=(R->mat[2][0])*((*P2)->mat[1][2]);
    matrice* Z2=matrice_nulle(3, 1);
    Z2->mat[0][0]=(R->mat[0][0]);
    X2->mat[1][0]=(R->mat[1][0]);
    X2->mat[2][0]=(R->mat[2][0]);
    matrice* tP=transposee(*P1);
    matrice* f1=resolution_systeme(tP,X2);
    matrice* f2=resolution_systeme(tP,Y2);
    matrice* f3=resolution_systeme(tP,Z2);
    matrice* res=transposee(concatenation3(f1,f2,f3));
    print_matrice(res);
    return res;
}

void test_F(double x1, double y1, double x2, double y2, matrice* F){
    matrice* test1=coo_vect(x1, y1);
    print_matrice(test1);
    matrice* wanted_result=coo_vect(x2, y2);
    matrice* res1=produit(F, wanted_result);
    multiplication_scalaire(res1, 1/res1->mat[2][0]);
    print_matrice(res1);
    printf("%f\n",norme_vecteur(res1,0));
    printf("------------- \n");
    free(test1);
    free(wanted_result);
}
int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2> [moravec/select]\n", argv[0]);
        return 1;
    }
    char command[500];
    char export_char[64];
    char points_file1[MAX_FILENAME], points_file2[MAX_FILENAME];
    char* filename1 = argv[1];
    char* filename2 = argv[2];

    snprintf(points_file1, sizeof(points_file1), "points_%s.txt", filename1);
    snprintf(points_file2, sizeof(points_file2), "points_%s.txt", filename2);

    int nbp2;
    matrice* input1 = read_jpg(filename1);
    matrice* input2 = read_jpg(filename2);
    matrice *img1, *img2;
    int** actif = NULL;
    printf("debug debut\n");
    fflush(stdout);
    int nbp1= moravec_arr(input1,&actif);
    printf("moravec : n detec = %d\n",nbp1);
    fflush(stdout);

    if (argc > 3 && strcmp(argv[3], "moravec") == 0) {
        img2 = selection_moravec(filename2, &nbp2, input2);
        printf("moravec2 : n detec = %d\n",nbp2);
        fflush(stdout);
        save_matrice_to_file_dimension(img2, points_file2);
        //filtrage sur la premiere image
        matrice* output1;
        output1=compute_score(input1,actif,nbp1);
        printf("compute_score done\n");
        fflush(stdout);
        nbp1 = filtre_mat(output1,actif,nbp1);
        printf("filtre done n garde = %d\n",nbp1);
        fflush(stdout);
        img1 = bit_image_to_points(output1,nbp1);
        free_matrice(output1);
    } else if (argc > 3 && strcmp(argv[3], "select") == 0) {
        snprintf(command, sizeof(command), "python3 save_points.py %s.jpg %s.jpg", filename1, filename2);
        system(command);
        read_matrice_from_file_dimension(&img1, points_file1);
        read_matrice_from_file_dimension(&img2, points_file2);
    } else {
        read_matrice_from_file_dimension(&img1, points_file1);
        read_matrice_from_file_dimension(&img2, points_file2);
    }

    nbp1 = img1->n;
    nbp2 = img2->n;

    snprintf(command, sizeof(command), "python3 plot_appariement.py %s.jpg %s.jpg points_%s.txt points_%s.txt", filename1, filename2, filename1, filename2);
    system(command);

    matrice* retenus = matrice_nulle(nbp1, 2);
    matrice* retenus_dh = matrice_nulle(nbp1, 1);
    uint256_t** retenus_descripteur = init_descriptor(nbp1);
    matrice* F = matrice_nulle(3, 3);
    read_matrice_from_file(F, "F.txt");
    srand(time(NULL));
    int pairs[NUM_PAIRS][4];
    generer_paires(pairs);
    uint256_t** img1_descripteur = compute_brief(input1, img1, pairs);
    uint256_t** img2_descripteur = compute_brief(input2, img2, pairs);
    for (int i = 0; i < nbp1; i++) {
        printf("\n-----------\n");
        printf("point %d de coordonnéees %d %d...\n", i, (int)img1->mat[i][0], (int)img1->mat[i][1]);
        matrice* X1 = coo_vect(img1->mat[i][0], img1->mat[i][1]);
        matrice* l = epipolar_line(F, X1);
        int h_min = 1000;

        for (int j = 0; j < nbp2; j++) {
            matrice* X2 = coo_vect(img2->mat[j][0], img2->mat[j][1]);
            if (fabs(produit_scalaire(l, X2)) < DISTANCE_SEUIL) {
                int h = hamming_distance(img2_descripteur[j], img1_descripteur[i]);
                if (h < h_min) {
                    retenus->mat[i][0] = img2->mat[j][0];
                    retenus->mat[i][1] = img2->mat[j][1];
                    retenus_dh->mat[i][0] = h;
                    h_min = h;
                }
            }
            free_matrice(X2);
        }
        free_matrice(X1);
        free_matrice(l);
    }

    // Suppression des correspondances aberrantes
    for (int i = 0; i < nbp1; i++) {
        if (retenus_dh->mat[i][0] > HAMMING_SEUIL) {
            retenus->mat[i][0] = -1;
            retenus->mat[i][1] = -1;
            img1->mat[i][0] = -1;
            img1->mat[i][1] = -1;
        }
    }

    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename1);
    save_matrice_to_file(img1, export_char);
    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename2);
    save_matrice_to_file(retenus, export_char);
    snprintf(command, sizeof(command), "python3 plot_points_ap.py %s.jpg %s.jpg points_ap_%s.txt points_ap_%s.txt", filename1, filename2, filename1, filename2);
    system(command);

    free(actif);
    return 0;
}


