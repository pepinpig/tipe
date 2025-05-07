#include "detection.h"

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
  char command[500];
  if (argc < 3) {
      fprintf(stderr, "Usage: %s <nom_image1> <nom_image2> [moravec/select]\n", argv[0]);
      return 1;
  }
  char export_char[64];
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  matrice* input1 = read_jpg(filename1);
  matrice* input2 = read_jpg(filename2);
  matrice *img1, *img2;

  if (argc > 3 && strcmp(argv[3], "moravec") == 0) {
    init_img_moravec(&img1,&img2,filename1,filename2,input1,input2);
    } else if (argc > 3 && strcmp(argv[3], "select") == 0) {
      init_img_select(&img1,&img2,filename1,filename2);
    } else {
      init_img_file(&img1,&img2,filename1,filename2);
    }

    int nbp1 = img1->n;
    int nbp2 = img2->n;
    matrice* retenus = corresp (img1,img2,input1,input2,nbp1,nbp2,filename1,filename2);
    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename1);
    save_matrice_to_file(img1, export_char);
    snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename2);
    save_matrice_to_file(retenus, export_char);
    if (argc > 3 && strcmp(argv[3], "moravec") == 0){
        snprintf(command, sizeof(command), "python3 plot_points_ap_inv.py %s.jpg %s.jpg points_ap_%s.txt points_ap_%s.txt", filename1, filename2, filename1, filename2);
    }
    else{
        snprintf(command, sizeof(command), "python3 plot_points_ap.py %s.jpg %s.jpg points_ap_%s.txt points_ap_%s.txt", filename1, filename2, filename1, filename2);
    }
    system(command);
    free_matrice(retenus);
    return 0;
}


