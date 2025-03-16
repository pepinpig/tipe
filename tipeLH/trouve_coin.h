#ifndef trouve_coin_H
#define trouve_coin_H

#include"moravec.h"
matrice* compute_score(matrice* plan, int** actif, int size);
void filtre_mat(matrice* input, int** actif, int size);

#endif
