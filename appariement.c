#include"appariement.h"

void generer_paires(int pairs[NUM_PAIRS][4]) {
    for (int i = 0; i < NUM_PAIRS; i++) {
        pairs[i][0] = rand() % PATCH_SIZE - PATCH_SIZE / 2; // x1
        pairs[i][1] = rand() % PATCH_SIZE - PATCH_SIZE / 2; // y1
        pairs[i][2] = rand() % PATCH_SIZE - PATCH_SIZE / 2; // x2
        pairs[i][3] = rand() % PATCH_SIZE - PATCH_SIZE / 2; // y2
    }
    printf("Paires BRIEF générées avec succés\n");
}

uint8_t get_pixel(matrice* image, int x, int y) {
    if (x<image->m&&y<image->n){
        return (uint8_t) image->mat[y][x];
    }
    else{
        printf("erreur: %d<%d ou %d<%d ",x, y, image->m, image->n);
        return (uint8_t) 0;
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
        res[i]->high1=0;
        res[i]->low1=0;
        res[i]->high2=0;
        res[i]->low2=0;
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
  // Fonction pour calculer les descripteurs BRIEF
uint256_t** compute_brief(matrice* image, matrice* points, int pairs[NUM_PAIRS][4]) {
    // Vérification de l'allocation mémoire pour res
    uint256_t** res = init_descriptor(points->n);
    if (res == NULL) {
        return NULL; // Retourne NULL si l'allocation échoue
    }
    // Parcours des points
    for (int i = 0; i < points->n; i++) {
        printf("calcul brief points %d\n",i);
        // Vérification que les coordonnées du point sont valides
        if (points->mat == NULL) {
            fprintf(stderr, "Points non valides\n");
            free(res);  // Libère la mémoire allouée avant de quitter
            return NULL;
        }
        int x = (int) points->mat[i][0]; 
        int y = (int) points->mat[i][1];
        printf("coordonées %d %d\n",x,y);
        // Parcours des paires
        for (int j = 0; j < NUM_PAIRS; j++) {
            int x1 = x + pairs[j][0], y1 = y + pairs[j][1];
            int x2 = x + pairs[j][2], y2 = y + pairs[j][3];
            // Vérification que les positions sont valides dans l’image
            if (x1 < 0 || y1 < 0 || x1 >= image->m || y1 >= image->n ||
                x2 < 0 || y2 < 0 || x2 >= image->m || y2 >= image->n) {
                continue; // Ignore cette paire
            }
            // Récupération des pixels de l'image
            uint8_t p1= get_pixel(image, x1, y1);
            uint8_t p2= get_pixel(image, x2, y2);
            // Mise à jour du descripteur BRIEF
            if (j < 64) {
                if (p1 < p2) {
                    res[i]->low1 |= (1ULL << j);
                }
            } 
            else { 
                if (j < 128) {
                    if (p1 < p2){
                        res[i]->high1 |= (1ULL << (j - 64));
                    } 
                }
                else {
                    if (j < 192) {
                        if (p1 < p2){
                            res[i]->low2 |= (1ULL << (j - 128));
                        } 
                    }
                    else {
                        if (p1 < p2){
                            res[i]->high2 |= (1ULL << (j - 192));
                        } 
                    }
                }
            }
            //printf("fin traitement pair %d", j);
        }
    }

    printf("Descripteurs BRIEF calculés\n");
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

    return one_count(xor1) + one_count(xor2)+one_count(xor3) + one_count(xor4);
}


matrice* epipolar_line(matrice* F, matrice* X){
    matrice* res=matrice_nulle(3,1);
    res = produit(F,X);
    multiplication_scalaire(res,100);
    return res;
}