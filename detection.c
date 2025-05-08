#include "detection.h" 

float point_line_distance(matrice* line, matrice* point) {
  float a = line->mat[0][0];
  float b = line->mat[1][0];
  float c = line->mat[2][0];
  float x = point->mat[0][0];
  float y = point->mat[1][0];
  return fabs(a * x + b * y + c) / sqrt(a * a + b * b);
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

matrice* bit_image_to_points (matrice* image, int nb_points){
    matrice* res=matrice_nulle(nb_points,2);
    int c=0;
    for (int i = 0; i < image->n; i++){
        for (int j=0; j<image->m; j++){
            if ((image->mat[i][j])==1){
                res->mat[c][0]=j;
                res->mat[c][1]=i;
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



void init_img_moravec(matrice** img1, matrice** img2, char* filename1, char* filename2, matrice* input1, matrice* input2){
  char points_file1[MAX_FILENAME], points_file2[MAX_FILENAME];
  snprintf(points_file1, sizeof(points_file1), "points_%s.txt", filename1);
  snprintf(points_file2, sizeof(points_file2), "points_%s.txt", filename2);
  int** actif = NULL;
  int nbp1= moravec_arr(input1,&actif);
  int nbp2;
  *img2 = selection_moravec(filename2, &nbp2, input2);
  save_matrice_to_file_dimension(*img2, points_file2);
  //filtrage sur la premiere image
  matrice* output1;
  output1=compute_score(input1,actif,nbp1);
  int nbp1bis = filtre_mat(output1,actif,nbp1);
  for(int i = 0;i<nbp1;i++){
    free(actif[i]);
  }
  free(actif);
  *img1 = bit_image_to_points(output1,nbp1bis);
  free_matrice(output1);
  save_matrice_to_file(*img1, points_file1);
}

void init_img_select(matrice** img1, matrice** img2, char* filename1, char* filename2){
  char command[500];
  char points_file1[MAX_FILENAME], points_file2[MAX_FILENAME];
  snprintf(points_file1, sizeof(points_file1), "points_%s.txt", filename1);
  snprintf(command, sizeof(command), "python3 save_points.py %s.jpg %s.jpg", filename1, filename2);
  system(command);
  read_matrice_from_file_dimension(img1, points_file1);
  read_matrice_from_file_dimension(img2, points_file2);
  snprintf(command, sizeof(command), "python3 plot_appariement.py %s.jpg %s.jpg points_%s.txt points_%s.txt", filename1, filename2, filename1, filename2);
  system(command);
}

void init_img_file(matrice** img1,matrice** img2, char* filename1, char* filename2){
  char points_file1[MAX_FILENAME], points_file2[MAX_FILENAME];
  snprintf(points_file1, sizeof(points_file1), "points_%s.txt", filename1);
  snprintf(points_file2, sizeof(points_file2), "points_%s.txt", filename2);
  read_matrice_from_file_dimension(img1, points_file1);
  read_matrice_from_file_dimension(img2, points_file2);
}

matrice* corresp (matrice* img1, matrice* img2, matrice* input1, matrice* input2,int nbp1,int nbp2,char* filename1, char* filename2){
  matrice* retenus = matrice_nulle(nbp1, 2);
  sortxy(img1);
  print_matrice(img1);
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
  sortxy(img1);
  int xprec=0;
  printf("n1 = %d, n2= %d",nbp1,nbp2);
  for (int i = 0; i < nbp1; i++) {
      //printf("\n-----------\n");
      //printf("point img1 %d de coordonnéees %d %d :\n", i, (int)img1->mat[i][0], (int)img1->mat[i][1]);
      matrice* X1 = coo_vect(img1->mat[i][0], img1->mat[i][1]);
      matrice* l = epipolar_line(F, X1);
      printf("epi line i = %d\n",i);
      //print_matrice(l);
      printf("\n",i);

      int h_min = 1000;

      for (int j = 0; j < nbp2; j++) {
        //printf("xprec = %d\n",xprec);
        if (img2->mat[j][0]>=xprec-MARGEX){
          matrice* X2 = coo_vect(img2->mat[j][0], img2->mat[j][1]);
          //printf("j : \n",j);
          //printf("X2 : \n");
          //print_matrice(X2);
          if (fabs(produit_scalaire(l, X2)) < DISTANCE_SEUIL) {
          printf("point img1 %d de coordonnéees %d %d :\n", i, (int)img1->mat[i][0], (int)img1->mat[i][1]);
          printf("point img2 %d de coordonnéees %d %d :", j, (int)img2->mat[j][0], (int)img2->mat[j][1]);
          printf("distance %f / hamming %d \n", fabs(produit_scalaire(l, X2)), hamming_distance(img2_descripteur[j], img1_descripteur[i]));
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
      }
      if (retenus_dh->mat[i][0] > HAMMING_SEUIL) {
        retenus->mat[i][0] = -1;
        retenus->mat[i][1] = -1;
        img1->mat[i][0] = -1;
        img1->mat[i][1] = -1;
      }
      else{
        xprec = retenus->mat[i][0];
      }
      free_matrice(X1);
      free_matrice(l);
  }
  return retenus;
}


