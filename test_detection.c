#include "detection.h"



int main(int argc, char* argv[]) {
    char command[500];
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2> [moravec/select]\n", argv[0]);
        return 1;
    }
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
        save_matrice_to_file_dimension(img1, points_file1);
        snprintf(command, sizeof(command), "python3 plot_appariement.py %s.jpg %s.jpg points_%s.txt points_%s.txt", filename1, filename2, filename1, filename2);
        //system(command);
    } else if (argc > 3 && strcmp(argv[3], "select") == 0) {
        snprintf(command, sizeof(command), "python3 save_points.py %s.jpg %s.jpg", filename1, filename2);
        system(command);
        read_matrice_from_file_dimension(&img1, points_file1);
        read_matrice_from_file_dimension(&img2, points_file2);
        snprintf(command, sizeof(command), "python3 plot_appariement.py %s.jpg %s.jpg points_%s.txt points_%s.txt", filename1, filename2, filename1, filename2);
        //system(command);
    } else {
      init_img_file(&img1,&img2,filename1,filename2);
    }

    nbp1 = img1->n;
    nbp2 = img2->n;
    matrice* retenus = matrice_nulle(nbp1, 2);
    matrice* retenus_dh = matrice_nulle(nbp1, 1);
    matrice* F = matrice_nulle(3, 3);
    printf("verifier d'avoir lancer correspondance.py pour clacluler F");
    char F_name[100];
    snprintf(F_name, sizeof(F_name), "F_%s.txt", filename1);
    read_matrice_from_file(F, F_name);
    srand(time(NULL));
    int pairs[NUM_PAIRS][4];
    generer_paires(pairs);
    uint256_t** img1_descripteur = compute_brief(input1, img1, pairs);
    uint256_t** img2_descripteur = compute_brief(input2, img2, pairs);
    int count=0;
    for (int i = 0; i < nbp1; i++) {
        matrice* X1 = coo_vect(img1->mat[i][0], img1->mat[i][1]);
        matrice* l = epipolar_line(F, X1);
        int h_min = HAMMING_SEUIL;
        int found=0;
        for (int j = 0; j < nbp2; j++) {
            matrice* X2 = coo_vect(img2->mat[j][0], img2->mat[j][1]);
            if (point_line_distance(l, X2) < DISTANCE_SEUIL) {
                found++;
                if (found==1){
                    count++;
                    //printf("\n-----------\n");
                    //printf("point img1 %d de coordonnéees %d %d :\n", count, (int)img1->mat[i][0], (int)img1->mat[i][1]);
                    //print_matrice(l);
                }
                //printf("point img2 %d de coordonnéees %d %d :", j, (int)img2->mat[j][0], (int)img2->mat[j][1]);
                //printf("distance %f / hamming %d \n", point_line_distance(l, X2), hamming_distance(img2_descripteur[j], img1_descripteur[i]));
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
        if (retenus->mat[i][0]==0) {
            retenus->mat[i][0] = -1;
            retenus->mat[i][1] = -1;
            img1->mat[i][0] = -1;
            img1->mat[i][1] = -1;
        }
    }

    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename1);
    int n1=save_matrice_to_file_clean(img1, export_char);
    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename2);
    int n2=save_matrice_to_file_clean(retenus, export_char);
    assert(n1==n2);
    printf("nb points appariés : %d\n", n1);
    snprintf(command, sizeof(command), "python3 plot_points_ap.py %s.jpg %s.jpg points_ap_%s.txt points_ap_%s.txt", filename1, filename2, filename1, filename2);
    //system(command);
    free(actif);
    return 0;
}


