#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define W 640   // Largeur de l'image
#define H 480  // Hauteur de l'image
#define SEUIL 100  // Seuil pour détecter un coin
#define WINDOW 1

void readImage(const char *filename, char image[H][W]) {
    FILE *file = fopen(filename, "rb"); // rb : lecture binaire
    if (fread(image, sizeof(unsigned char), W * H, file) != W*H) {
        perror("Erreur lors de la lecture de l'image");
        fclose(file);
    }
    fclose(file);
}

void writeImage(const char *filename, char image[H][W]) {
    FILE *file = fopen(filename, "wb"); // wb : écriture binaire
    if (fwrite(image, sizeof(unsigned char), W * H, file) != W * H) {
        perror("Erreur lors de l'écriture de l'image");
        fclose(file);
    }
    fclose(file);
}

// Calcul de la variance d'intensité dans une direction donnée
int variance(char image[H][W], int x, int y, int dx, int dy) {
    int sum = 0, sumSq = 0, count = 0;

    // Parcourir une fenêtre de 3x3 dans la direction donnée
    for (int i = -WINDOW; i <= WINDOW; ++i) {
        int nx = x + i * dx;
        int ny = y + i * dy;

        // Vérifier les limites de l'image
        if (nx >= 0 && nx < W && ny >= 0 && ny < H) {
            int val = image[ny][nx];
            sum += val;
            sumSq += val * val;
            count++;
        }
    }
    // Retourner la variance, ou 0 si aucune donnée valide
    return (count > 0) ? (sumSq - (sum * sum) / count) : 0;
}

// Algorithme de Moravec
void moravec(char input[H][W], char output[H][W]) {
    // Initialiser l'image de sortie avec l'image d'entrée
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            output[y][x] = input[y][x];
        }
    }

    // Appliquer l'algorithme de Moravec
    for (int y = 1; y < H - 1; ++y) {
        for (int x = 1; x < W - 1; ++x) {
            // Calculer la variance dans 4 directions
            int var1 = variance(input, x, y, 1, 0);  // Droite/Gauche
            int var2 = variance(input, x, y, 0, 1);  // Haut/Bas
            int var3 = variance(input, x, y, 1, 1);  // Diagonale bas-droite/haut-gauche
            int var4 = variance(input, x, y, 1, -1); // Diagonale bas-gauche/haut-droite

            // Trouver le minimum des variances
            int minvar = fmin(fmin(var1, var2), fmin(var3, var4));

            // Appliquer le seuil pour marquer un coin
            if (minvar > SEUIL) {
                output[y][x] = 255;  // Coin détecté
            } else {
                output[y][x] = 0;    // Pas de coin
            }
        }
    }
}

int main() {
    char input[H][W];
    char output[H][W];
    readImage("points/images/dodec1.pgm", input);
    moravec(input, output);
    writeImage("points/images/output.pgm", output);
    return 0;
}
