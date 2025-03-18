#include"appariement.h"

void generer_paires(int pairs[NUM_PAIRS][4]) {
    for (int i = 0; i < NUM_PAIRS; i++) {
        pairs[i][0] = rand() % PATH_SIZE - PATH_SIZE / 2; // x1
        pairs[i][1] = rand() % PATH_SIZE - PATH_SIZE / 2; // y1
        pairs[i][2] = rand() % PATH_SIZE - PATH_SIZE / 2; // x2
        pairs[i][3] = rand() % PATH_SIZE - PATH_SIZE / 2; // y2
    }
}

uint8_t get_pixel(matrice* image, int x, int y) {
    if (x < 0 || x >= image->m || y < 0 || y >= image->n) return 0;
    return (uint8_t) image->mat[y][x];
}
uint128_t* compute_brief(matrice* image, matrice* points, int pairs[NUM_PAIRS][4]) {
    uint128_t* res = malloc(points->n * sizeof(uint128_t));

    for (int i = 0; i < points->n; i++) {
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];

        uint128_t descriptor = {0, 0}; // Initialise les deux parties à 0

        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];

            uint8_t p1 = get_pixel(image, x1, y1);
            uint8_t p2 = get_pixel(image, x2, y2);

            if (j < 64) {
                if (p1 < p2) descriptor.low |= (1ULL << j);
            } else {
                if (p1 < p2) descriptor.high |= (1ULL << (j - 64));
            }
        }

        res[i] = descriptor; // Stocke le descripteur
    }

    return res;
}

int one_count(uint64_t x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;         
    }
    return count;
}

int hamming_distance(uint128_t d1, uint128_t d2) {
    uint64_t xor1 = d1.high ^ d2.high;
    uint64_t xor2 = d1.low ^ d2.low;

    return one_count(xor1) + one_count(xor2);
}


matrice* epipolar_line(matrice* F, matrice* X){
    matrice* res=matrice_nulle_pointeur(3,1);
    *(res->mat)=produit(*(F->mat),*(X->mat));
    return res;
}