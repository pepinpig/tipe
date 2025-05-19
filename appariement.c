#include "appariement.h"


void generer_paires(int pairs[NUM_PAIRS][4]) {
    for (int i = 0; i < NUM_PAIRS; i++) {
        pairs[i][0] = rand() % PATCH_SIZE - PATCH_SIZE / 2;
        pairs[i][1] = rand() % PATCH_SIZE - PATCH_SIZE / 2;
        pairs[i][2] = rand() % PATCH_SIZE - PATCH_SIZE / 2;
        pairs[i][3] = rand() % PATCH_SIZE - PATCH_SIZE / 2;
    }
    printf("Paires BRIEF générées avec succés\n");
}

uint8_t get_pixel(matrice* image, int x, int y) {
    if (x < image->m && y < image->n) {
        return (uint8_t) image->mat[y][x];
    } else {
        printf("erreur: %d<%d ou %d<%d ", x, y, image->m, image->n);
        return (uint8_t) 0;
    }
}

pixel_rgb get_pixel_rgb(matrice* image_r,matrice* image_g, matrice* image_b, int x, int y) {
    pixel_rgb px = {0,0,0};
    if (x < image_r->m && y < image_r->n) {
        px.r = (uint8_t)image_r->mat[y][x];
        px.g = (uint8_t)image_g->mat[y][x];
        px.b = (uint8_t)image_b->mat[y][x];
        return px;
    } else {
        printf("erreur: %d<%d ou %d<%d ", x, y, image_r->m, image_r->n);
        return px;
    }
}

uint256_t** init_descriptor(int n) {
    uint256_t** res = malloc(n * sizeof(uint256_t*));
    if (res == NULL) {
        fprintf(stderr, "Erreur d'allocation pour le tableau de descripteurs\n");
        return NULL; 
    }
    for (int i = 0; i < n; i++) {
        res[i] = malloc(sizeof(uint256_t));
        res[i]->high1 = 0;
        res[i]->low1 = 0;
        res[i]->high2 = 0;
        res[i]->low2 = 0;
    }
    return res;
}

void free_descriptors(uint256_t** res, int n) {
    if (res != NULL) {
        for (int i = 0; i < n; i++) {
            free(res[i]); 
        }
        free(res);
    }
}

uint256_t** compute_brief(matrice* image, matrice* points, int pairs[NUM_PAIRS][4]) {
    uint256_t** res = init_descriptor(points->n);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < points->n; i++) {
        if (points->mat == NULL) {
            fprintf(stderr, "Points non valides\n");
            free(res);  
            return NULL;
        }
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];
        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];
            if (x1 < 0 || y1 < 0 || x1 >= image->m || y1 >= image->n ||
                x2 < 0 || y2 < 0 || x2 >= image->m || y2 >= image->n) {
                continue;
            }
            uint8_t p1 = get_pixel(image, x1, y1);
            uint8_t p2 = get_pixel(image, x2, y2);
            if (j < 64) {
                if (p1 < p2) {
                    res[i]->low1 |= (1ULL << j);
                }
            } else { 
                if (j < 128) {
                    if (p1 < p2) {
                        res[i]->high1 |= (1ULL << (j - 64));
                    } 
                } else {
                    if (j < 192) {
                        if (p1 < p2) {
                            res[i]->low2 |= (1ULL << (j - 128));
                        } 
                    } else {
                        if (p1 < p2) {
                            res[i]->high2 |= (1ULL << (j - 192));
                        } 
                    }
                }
            }
        }
    }
    printf("Descripteurs BRIEF calculés\n");
    return res;
}


uint256_t** compute_brief_rgb(matrice* image_r,matrice* image_g,matrice* image_b, matrice* points, int pairs[NUM_PAIRS][4]) {
    uint256_t** res = init_descriptor(points->n);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < points->n; i++) {
        if (points->mat == NULL) {
            fprintf(stderr, "Points non valides\n");
            free(res);  
            return NULL;
        }
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];
        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];
            if (x1 < 0 || y1 < 0 || x1 >= image_r->m || y1 >= image_r->n ||
                x2 < 0 || y2 < 0 || x2 >= image_r->m || y2 >= image_r->n) {
                continue;
            }
            pixel_rgb p1 = get_pixel_rgb(image_r,image_g,image_b, x1, y1);
            pixel_rgb p2 = get_pixel_rgb(image_r,image_g,image_b, x2, y2);
            if (p1.r < p2.r) res[i]->low1 |= (1ULL << j);
            if (p1.g < p2.g) res[i]->low2 |= (1ULL << j);
            if (p1.b < p2.b) res[i]->high1 |= (1ULL << j);
        }
    }
    printf("Descripteurs BRIEF_rgb calculés\n");
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

int hamming_distance(uint256_t* d1, uint256_t* d2) {
    uint64_t xor1 = d1->high1 ^ d2->high1;
    uint64_t xor2 = d1->low1 ^ d2->low1;
    uint64_t xor3 = d1->high2 ^ d2->high2;
    uint64_t xor4 = d1->low2 ^ d2->low2;
    return 2*one_count(xor1) + 2*one_count(xor2) + 2*one_count(xor3) + one_count(xor4);
}

matrice* epipolar_line(matrice* F, matrice* X) {
    matrice* res = matrice_nulle(3, 1);
    res = produit(F, X);
    multiplication_scalaire(res, 100);
    return res;
}
