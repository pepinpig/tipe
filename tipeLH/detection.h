#ifndef detection_H
#define detection_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "matrice.h"
#include "moravec.h"
#include "brief_descriptor.h"
#include "epipolar_distance.h"
#include "manipulation_fichiers.h"
#include "detection.h"


// matrice* detection(char* filename, int window, int seuil, int param);

// matrice* detection_impression(char* filename, int window, int seuil, int param);
// Fonction pour calculer la distance de Hamming entre deux descripteurs
int hamming_distance(uint64_t descriptor1[2], uint64_t descriptor2[2]);

// Fonction pour calculer la droite épipolaire pour un point donné
void epipolar_line();

// Fonction pour calculer la distance d'un point à une droite épipolaire
float distance_to_epipolar_line ();

// Fonction pour sélectionner la meilleure correspondance entre deux images
int select_best_match(int n1, int n2, float F[3][3]);

#endif