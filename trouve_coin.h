#pragma once
#include"moravec.h"
matrice* compute_score(matrice* plan, int** actif, int size);
void filtre_mat(matrice* input, int** actif, int size);
