#include "detection.h"

int main(int argc, char* argv[]) {
  char command[500];
  if (argc < 3) {
      fprintf(stderr, "Usage: %s <nom_image1> <nom_image2>\n", argv[0]);
      return 1;
  }
  char export_char[64];
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  matrice* input1 = read_jpg(filename1);
  matrice* input2 = read_jpg(filename2);
  matrice* input1_r = read_jpg_color(filename1, "r");
  matrice* input1_g = read_jpg_color(filename1, "g");
  matrice* input1_b = read_jpg_color(filename1, "b");
  matrice* input2_r = read_jpg_color(filename2, "r");
  matrice* input2_g = read_jpg_color(filename2, "g");
  matrice* input2_b = read_jpg_color(filename2, "b");

  matrice *img1, *img2;
  init_img_moravec(&img1,&img2,filename1,filename2,input1,input2);

  int nbp1 = img1->n;
  int nbp2 = img2->n;
  matrice* retenus = corresp_color (img1,img2,input1_r, input1_g, input1_b, input2_r, input2_g, input2_b, nbp1,nbp2,filename1,filename2);
  snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename1);
  int n1=save_matrice_to_file_clean(img1, export_char);
  if (n1==0||n1==1){
    fprintf(stderr, "Erreur : pas de points détéctés.\n");
    exit(EXIT_FAILURE); 
  }
  snprintf(export_char, sizeof(export_char), "points_ap_%s.txt", filename2);
  int n2=save_matrice_to_file_clean(retenus, export_char);
  assert(n1==n2);
  snprintf(command, sizeof(command), "python3 plot_points_ap.py %s.jpg %s.jpg points_ap_%s.txt points_ap_%s.txt", filename1, filename2, filename1, filename2);
  system(command);
  free_matrice(retenus);
  return 0;
}