#ifndef BRIEF_DESCRIPTOR_H
#define BRIEF_DESCRIPTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrice.h" 

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



#endif // BRIEF_DESCRIPTOR_H
