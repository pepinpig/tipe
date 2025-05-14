#include "moravec.h"
#include "appariement.h"
#include "trouve_coin.h"
#include "camera_calibration.h"

#define MAX_FILENAME 256
#define MARGEX 0

float point_line_distance(matrice* line, matrice* point);
matrice* read_jpg(char* filename);
matrice* bit_image_to_points (matrice* image, int nb_points);
matrice* selection_moravec(char* filename, int* nbp, matrice* input);
void init_img_moravec(matrice** img1, matrice** img2, char* filename1, char* filename2, matrice* input1, matrice* input2);
void init_img_select(matrice** img1, matrice** img2, char* filename1, char* filename2);
void init_img_file(matrice** img1,matrice** img2, char* filename1, char* filename2);
matrice* corresp (matrice* img1, matrice* img2, matrice* input1, matrice* input2,int nbp1,int nbp2,char* filename1, char* filename2);
