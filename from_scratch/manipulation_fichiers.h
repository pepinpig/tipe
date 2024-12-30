#pragma once

// Crée une matrice avec les coefficients dans nom fichier
matrice lire_matrice(char* nom_fichier);

// Charger les points d'une image depuis un fichier
void load_all_points_images(const char* filename, long double* u, long double*v);

// Charger les points réels depuis un fichier
void load_all_points_reels(const char* filename, long double* X, long double* Y, long double* Z);

//Enregistre la matrice dans un fichier texte <matrix_name>-<image_name>.txt avec ses dimensions sur la première ligne
void save_matrice_to_file_dimension(matrice* matrix, char* filename);
//Enregsitre la matrice dans un fichier texte <matrix_name>-<image_name>.txt
void save_matrice_to_file(matrice* matrix, char* filename);

//Lis la matrice dans filename avec les dimension sur le première ligne et fait pointer vers mtx
void read_matrice_from_file_dimension(matrice* mtx, char* filename);
//Lis la matrice dans filename et fait pointer vers mtx
void read_matrice_from_file(matrice *A, const char *filename);
//Créer chaine de caractère <matrix_name>-<image_name>.txt
char* nom_fichier(char* filename, char* matrix_name, char* image_name);