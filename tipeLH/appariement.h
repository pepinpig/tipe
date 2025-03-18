#ifndef APPARIEMENT_H
#define APPARIEMENT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrice.h" 
#include "camera_calibration.h" 


// Définitions des constantes
#define PATCH_SIZE 31   // Taille du patch utilisée
#define NUM_BITS 256    // Nombre de bits pour le descripteur
#define NUM_PAIRS (NUM_BITS / 2)  // 128 paires pour remplir deux colonnes

// Déclaration des fonctions
// Générer des paires de tests BRIEF avec des paramètres pour la taille de patch et le nombre de bits
void generer_paires(int pairs[NUM_PAIRS][4], int patch_size);

// Récupérer l'intensité d'un pixel dans l'image (grayscale 2D array)
uint8_t get_pixel(matrice* image, int x, int y);

// Générer le descripteur BRIEF et l'ajouter à la matrice "points"
void compute_brief(matrice* image, matrice* points, int patch_size, int num_bits);


/ matrice* detection(char* filename, int window, int seuil, int param);

// matrice* detection_impression(char* filename, int window, int seuil, int param);
// Fonction pour calculer la distance de Hamming entre deux descripteurs
int hamming_distance(uint64_t descriptor1[2], uint64_t descriptor2[2]);

// Fonction pour calculer la droite épipolaire pour un point donné
matrice* epipolar_line(matrice* F, long double x, long double y);

#endif 
