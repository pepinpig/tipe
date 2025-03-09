#define WINDOW 2 // nombre de pixel considéré dans chaque direction
#define SEUIL 1000 
#define PARAM true 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "matrice.h"
#include "manipulation_fichiers.h"
#include "detection.h"

int hamming_distance(uint64_t descriptor1[2], uint64_t descriptor2[2]) {
    int distance = 0;
    for (int i = 0; i < 2; i++) {
        uint64_t xor_result = descriptor1[i] ^ descriptor2[i];  // XOR des deux descripteurs
        while (xor_result) {
            distance += xor_result & 1;  // Compter les bits à 1
            xor_result >>= 1;
        }
    }
    return distance;
}


// Structure pour un point 2D
typedef struct {
    float x;
    float y;
} Point;

// Fonction pour calculer la droite épipolaire pour un point donné dans l'image 1
void epipolar_line(float F[3][3], Point p1, float line[3]) {
    // Conversion du point en homogène (x, y, 1)
    float point_homogeneous[3] = { p1.x, p1.y, 1.0f };
    
    // Multiplication de la matrice fondamentale F avec le point homogène
    for (int i = 0; i < 3; i++) {
        line[i] = 0;
        for (int j = 0; j < 3; j++) {
            line[i] += F[i][j] * point_homogeneous[j];
        }
    }
}

// Fonction pour calculer la distance d'un point à une droite épipolaire
float distance_to_epipolar_line(float line[3], Point p) {
    float a = line[0];
    float b = line[1];
    float c = line[2];
    
    // Calcul de la distance
    return fabs(a * p.x + b * p.y + c) / sqrt(a * a + b * b);
}

// Structure pour une correspondance entre deux points
typedef struct {
    Point p1;
    Point p2;
} Match;

// Fonction pour sélectionner la meilleure correspondance entre deux images
Match select_best_match(Point *points1, unsigned char **descriptors1, int n1,
                        Point *points2, unsigned char **descriptors2, int n2, float F[3][3]) {
    Match best_match = {{0, 0}, {0, 0}};
    int min_hamming_distance = INT_MAX;
    float min_distance_to_line = FLT_MAX;
    
    for (int i = 0; i < n1; i++) {
        // Calcul de la droite épipolaire associée au point dans l'image 1
        float line2[3];
        epipolar_line(F, points1[i], line2);
        
        // Sélectionner les correspondances dans l'image 2
        for (int j = 0; j < n2; j++) {
            // Calcul de la distance du point de l'image 2 à la droite épipolaire
            float distance = distance_to_epipolar_line(line2, points2[j]);
            
            // Comparer les descripteurs si la distance épipolaire est faible
            if (distance < min_distance_to_line) {
                int hamming_dist = hamming_distance(descriptors1[i], descriptors2[j], 256); // Exemple pour descripteurs de 256 bits
                if (hamming_dist < min_hamming_distance) {
                    best_match.p1 = points1[i];
                    best_match.p2 = points2[j];
                    min_hamming_distance = hamming_dist;
                    min_distance_to_line = distance;
                }
            }
        }
    }
    
    return best_match;
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_image>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    char input_name[64];
    snprintf(input_name, sizeof(input_name), "%s.txt", filename);
