#include"appariement.h"

void generer_paires(int pairs[NUM_PAIRS][4], int patch_size) {
    for (int i = 0; i < NUM_PAIRS; i++) {
        pairs[i][0] = rand() % patch_size - patch_size / 2; // x1
        pairs[i][1] = rand() % patch_size - patch_size / 2; // y1
        pairs[i][2] = rand() % patch_size - patch_size / 2; // x2
        pairs[i][3] = rand() % patch_size - patch_size / 2; // y2
    }
}

uint8_t get_pixel(matrice* image, int x, int y) {
    if (x < 0 || x >= image->m || y < 0 || y >= image->n) return 0;
    return (uint8_t) image->mat[y][x];
}

matrice* compute_brief(matrice* image, int pairs[NUM_PAIRS][4]) {
    for (int i = 0; i < points->n; i++) {
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];

        uint64_t descriptor1 = 0;
        uint64_t descriptor2 = 0;

        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];

            uint8_t p1 = get_pixel(image, x1, y1);
            uint8_t p2 = get_pixel(image, x2, y2);

            if (j < 64) {
                if (p1 < p2) descriptor1 |= (1ULL << j);
            } else {
                if (p1 < p2) descriptor2 |= (1ULL << (j - 64));
            }
        }

        // Stocker les deux parties du descripteur dans les colonnes 2 et 3
        points->mat[i][2] = (long double) descriptor1;
        points->mat[i][3] = (long double) descriptor2;
    }
}


matrice* epipolar_line(matrice* F, matrice* X){
    matrice* res=matrice_nulle_pointeur(3,1);
    *(res->mat)=produit(*(F->mat),*(X->mat));
    return res;
}