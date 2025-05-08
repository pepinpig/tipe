#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichier.h"

// #define WINDOW 5 //bon seuil
// #define SEUIL 1000
// #define SEUILTC 1000
// #define DIST 40

#define WINDOW 5 // nombre de pixel considéré dans chaque direction
#define SEUIL 2000
#define SEUILTC 50
#define DIST 20

#define PARAM true 

double variance(matrice* image, int x, int y, int dx, int dy);
int moravec(matrice* image_input, matrice* image_output);
int moravec_arr(matrice* image_input, int*** detected);
