#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrice.h" 
#include <stdbool.h> 
#include "time.h" 
#include "camera_calibration.h" 


// Définitions des constantes
#define PATCH_SIZE 51   // Taille du patch utilisée
#define NUM_PAIRS 256  

typedef struct uint256{
    uint64_t high1;  // Partie haute (64 bits)
    uint64_t low1;   // Partie basse (64 bits)
    uint64_t high2;  // Partie haute (64 bits)
    uint64_t low2;   // Partie basse (64 bits)
} uint256_t;

typedef struct {
    uint8_t r, g, b;
} pixel_rgb;

// Déclaration des fonctions
// Générer des paires de tests BRIEF avec des paramètres pour la taille de patch et le nombre de bits
void generer_paires(int pairs[NUM_PAIRS][4]);

// Récupérer l'intensité d'un pixel dans l'image (grayscale 2D array)
uint8_t get_pixel(matrice* image, int x, int y);

// Récupérer l'intensité d'un pixel dans l'image couleur
pixel_rgb get_pixel_rgb(matrice* image_r,matrice* image_g, matrice* image_b, int x, int y) ;

// Fonction pour initialiser un tableau de descripteurs BRIEF
uint256_t** init_descriptor(int n);

// Fonction pour libérer la mémoire allouée à un tableau de descripteurs BRIEF
void free_descriptors(uint256_t** res, int n);

// Générer le descripteur BRIEF et l'ajouter à la matrice "points"
uint256_t** compute_brief(matrice* image, matrice* points, int pairs[NUM_PAIRS][4] );

// Générer le descripteur BRIEF_RGB et l'ajouter à la matrice "points"
uint256_t** compute_brief_rgb(matrice* image_r,matrice* image_g,matrice* image_b, matrice* points, int pairs[NUM_PAIRS][4]);
 
// Compte le nombre de bits à 1 dans un uint64_t
int one_count(uint64_t x);

// Calcule la distance de Hamming entre deux uint256_t
int hamming_distance(uint256_t* d1, uint256_t* d2);

// Fonction pour calculer la droite épipolaire pour un point donné
matrice* epipolar_line(matrice* F, matrice* X);
