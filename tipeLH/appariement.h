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
#define PATCH_SIZE 31   // Taille du patch utilisée
#define NUM_BITS 256    // Nombre de bits pour le descripteur
#define NUM_PAIRS (NUM_BITS / 2)  // 128 paires pour remplir deux colonnes

typedef struct {
    uint64_t high;  // Partie haute (64 bits)
    uint64_t low;   // Partie basse (64 bits)
} uint128_t;


// Déclaration des fonctions
// Générer des paires de tests BRIEF avec des paramètres pour la taille de patch et le nombre de bits
void generer_paires(int pairs[NUM_PAIRS][4]);

// Récupérer l'intensité d'un pixel dans l'image (grayscale 2D array)
uint8_t get_pixel(matrice* image, int x, int y);

// Générer le descripteur BRIEF et l'ajouter à la matrice "points"
uint128_t* compute_brief(matrice* image, matrice* points, int pairs[NUM_PAIRS][4] );

// Compte le nombre de bits à 1 dans un uint64_t
int one_count(uint64_t x);

// Calcule la distance de Hamming entre deux uint128_t
int hamming_distance(uint128_t d1, uint128_t d2);

// Fonction pour calculer la droite épipolaire pour un point donné
matrice* epipolar_line(matrice* F, matrice* X);
