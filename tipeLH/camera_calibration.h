#ifndef CAMERA_CALIBRATION_H
#define CAMERA_CALIBRATION_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichier.h"
#include "SVD.h"

// Fonction pour construire la matrice A
matrice* construction_A(long double* X, long double* Y, long double* Z, long double* u, long double* v);

// Fonction pour résoudre la calibration de la caméra
void camera_calibration_resolution(matrice* P, matrice* A);

//Calcule la matrice fondamental F associé à l'image
matrice* compute_F(char* filename1, char* filename2);

#endif // CAMERA_CALIBRATION_H