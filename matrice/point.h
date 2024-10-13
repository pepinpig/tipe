#pragma once

typedef enum {
    REEL,
    PROJECTIF,
    CAMERA_1,
    CAMERA_2
} CoordType;


// Structure globale pour stocker toutes les informations d'un point
typedef struct {
    matrice reel;          // Vecteur coordonnées dans l'espace réel matrice 3*1
    matrice proj;          // Coordonnées dans l'espace projectif matrice 3*1
    matrice cam1;          // Coordonnées sur le plan de la caméra 1 matrice 2*1
    matrice cam2;          // Coordonnées sur le plan de la caméra 2 matrice 2*1
} Point;

//Initialiser un point
Point point_vide();

//Initialiser un point
Point* point_vide_pointeur();

//Convertir point plan en point 3D
matrice vect3_of_vect2(matrice v);

