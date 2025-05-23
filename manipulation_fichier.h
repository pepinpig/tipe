#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "matrice.h"

//Teste l'existence du fichier filename
bool file_exists(const char *filename);

// Charger les points d'une image depuis un fichier
void load_all_points_images(const char* filename, double* u, double*v, int n);

// Charger les points réels depuis un fichier
void load_all_points_reels(const char* filename, double* X, double* Y, double* Z, int n);

//Enregistre la matrice dans un fichier texte <matrix_name>-<image_name>.txt avec ses dimensions sur la première ligne
void save_matrice_to_file_dimension(matrice* matrix, char* filename);

void save_matrice_pbm(matrice* matrix, char* filename, char* parametre);
//Enregsitre la matrice dans un fichier texte <matrix_name>-<image_name>.txt
void save_matrice_to_file(matrice* matrix, char* filename);
//Comme save_matrice to file mais elimine les lignes commençant par -1
int save_matrice_to_file_clean(matrice* matrix, char* filename);

int save_matrice_to_file_clean_dimension(matrice *A, char* filename);
//Lis la matrice dans filename avec les dimension sur le première ligne et fait pointer vers mtx
void read_matrice_from_file_dimension(matrice** mtx, char* filename);

//Lis la matrice dans filename et fait pointer vers mtx
void read_matrice_from_file(matrice *A, const char *filename);
//Créer chaine de caractère <matrix_name>-<image_name>.txt
void nom_fichier(char* filename, char* matrix_name, char* image_name);
