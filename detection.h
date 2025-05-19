#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"
#include "ransac.h"

float point_line_distance(matrice* line, matrice* point);

matrice* bit_image_to_points (matrice* image, int nb_points);

matrice* selection_moravec(char* filename, int* nbp, matrice* input);

matrice* read_jpg_color(char* filename, char* color);

void init_img_moravec(matrice** img1, matrice** img2, char* filename1, char* filename2, matrice* input1, matrice* input2);

void init_img_file(matrice** img1,matrice** img2, char* filename1, char* filename2);

matrice* corresp_color (matrice* img1, matrice* img2, matrice* input1_r,matrice* input1_g,matrice* input1_b, matrice* input2_r,matrice* input2_g, matrice* input2_b, int nbp1,int nbp2,char* filename1, char* filename2);