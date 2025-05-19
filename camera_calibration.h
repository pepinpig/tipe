#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichier.h"
#include "SVD.h"
#define N 16 //nombre de points de calibration
// Fonction pour construire la matrice A
matrice* construction_A(double* X, double* Y, double* Z, double* u, double* v, int n);

// Fonction pour résoudre la calibration de la caméra
void camera_calibration_resolution(matrice* P, matrice* A, matrice* K, matrice* R, matrice* T);

//Calcule la matrice fondamental F associé aux images
matrice* compute_F(matrice* K1 ,matrice* R1, matrice* T1, matrice* K2, matrice* R2, matrice* T2);