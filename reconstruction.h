#pragma once
#include "camera_calibration.h"

int reconstruction1(const char* image_name1, const char* image_name2, matrice** mat);
int reconstruction1_aux(matrice* P1, matrice* P2,const char* file_points1, const char* file_points2, matrice** mat, const char* image_name1) ;
int reconstruction3(const char* in1, const char* in2,  const char* in3,  const char* in4,  const char* in5,const char* in6, matrice** mat);