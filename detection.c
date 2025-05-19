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

matrice* read_jpg_color(char* filename, char* color){
  char input_name[32];
  snprintf(input_name, sizeof(input_name), "%s_%s.txt", filename, color);
  if (!file_exists(input_name)){
      char command[128];
      snprintf(command, sizeof(command), "python3 jpg_to_txt_color.py %s.jpg", filename);
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
    snprintf(parametre, sizeof(parametre), "fichier:%s, seuil:%d, fenetre:%d, param:%d", filename, Seuil_moravec, Window, PARAM);
    //snprintf(output_name, sizeof(output_name), "%s-mv-%d-%d-%d.pbm", filename, Seuil_moravec, Window, PARAM);
    //save_matrice_pbm(output, output_name, parametre);
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
  char command[256];
  int nbp1;
  int nbp2;
  *img1 =selection_moravec(filename1, &nbp1, input1);
  *img2 = selection_moravec(filename2, &nbp2, input2);
  save_matrice_to_file_dimension(*img1, points_file1);
  snprintf(command, sizeof(command), "python3 plot_appariement_un.py %s.jpg points_%s.txt", filename1,  filename1);
  //system(command);

  //trouve coin
  int** actif = NULL;
  nbp1= moravec_arr(input1,&actif);
  matrice* output1;
  output1=compute_score(input1,actif,nbp1);
  int nbp1bis = filtre_mat(output1,actif,nbp1);
  for(int i = 0;i<nbp1;i++){
    free(actif[i]);
  }
  free(actif);
  *img1 = bit_image_to_points(output1,nbp1bis);
  free_matrice(output1);
  save_matrice_to_file_dimension(*img1, points_file1);
  //system(command);

  //filtrage sur la premiere image
  detect_lines_and_extremities(*img1);
  save_matrice_to_file_clean_dimension(*img1, points_file1);
  save_matrice_to_file_dimension(*img2, points_file2);
  //system(command);
}

void init_img_select(matrice** img1, matrice** img2, char* filename1, char* filename2){
  char command[500];
  char points_file1[MAX_FILENAME], points_file2[MAX_FILENAME];
  snprintf(points_file1, sizeof(points_file1), "points_%s.txt", filename1);
  snprintf(points_file2, sizeof(points_file2), "points_%s.txt", filename2);
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
  int count=0;
  for (int i = 0; i < nbp1; i++) {
    int found=0;
    //printf("\n-----------\n");
    //printf("point img1 %d de coordonnéees %d %d :\n", i, (int)img1->mat[i][0], (int)img1->mat[i][1]);
    matrice* X1 = coo_vect(img1->mat[i][0], img1->mat[i][1]);
    matrice* l = epipolar_line(F, X1);
    int h_min = Hamming_seuil;
    for (int j = 0; j < nbp2; j++) {
      //printf("xprec = %d\n",xprec);
      matrice* X2 = coo_vect(img2->mat[j][0], img2->mat[j][1]);
      //printf("j : \n",j);
      //printf("X2 : \n");
      //print_matrice(X2);
      if (point_line_distance(l, X2) < Distance_seuil) {
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
          printf("\n\n\n\n");
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
      else{
        xprec = retenus->mat[i][0];
      }
  }
  return retenus;
}

matrice* corresp_color (matrice* img1, matrice* img2, matrice* input1_r,matrice* input1_g,matrice* input1_b, matrice* input2_r,matrice* input2_g, matrice* input2_b, int nbp1,int nbp2,char* filename1, char* filename2){
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
  uint256_t** img1_descripteur = compute_brief_rgb(input1_r,input1_g,input1_b,img1, pairs);
  uint256_t** img2_descripteur = compute_brief_rgb(input2_r,input2_g,input2_b, img2, pairs);
  sortxy(img1);
  int xprec=0;
  printf("n1 = %d, n2= %d",nbp1,nbp2);
  int count=0;
  for (int i = 0; i < nbp1; i++) {
    int found=0;
    //printf("\n-----------\n");
    //printf("point img1 %d de coordonnéees %d %d :\n", i, (int)img1->mat[i][0], (int)img1->mat[i][1]);
    matrice* X1 = coo_vect(img1->mat[i][0], img1->mat[i][1]);
    matrice* l = epipolar_line(F, X1);
    int h_min = Hamming_seuil;
    for (int j = 0; j < nbp2; j++) {
      //printf("xprec = %d\n",xprec);
      if (img2->mat[j][0]>=xprec-MARGEX){
        matrice* X2 = coo_vect(img2->mat[j][0], img2->mat[j][1]);
        //printf("j : \n",j);
        //printf("X2 : \n");
        //print_matrice(X2);
        if (point_line_distance(l, X2) < Distance_seuil) {
          found++;
          if (found==1){
            count++;
            //printf("\n-----------\n");
            //printf("point img1 %d de coordonnéees %d %d :\n", count, (int)img1->mat[i][0], (int)img1->mat[i][1]);
            //print_matrice(l);
          }
          int h = hamming_distance(img2_descripteur[j], img1_descripteur[i]);
          if (h < h_min) {
            //printf("point img2 %d de coordonnéees %d %d :", j, (int)img2->mat[j][0], (int)img2->mat[j][1]);
            //printf("distance %f / hamming %d \n", point_line_distance(l, X2), hamming_distance(img2_descripteur[j], img1_descripteur[i]));
            retenus->mat[i][0] = img2->mat[j][0];
            retenus->mat[i][1] = img2->mat[j][1];
            retenus_dh->mat[i][0] = h;
            h_min = h;
          }
        }
        free_matrice(X2);
      }
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
      else{
        xprec = retenus->mat[i][0];
      }
  }
  return retenus;
}