#pragma once
#include"moravec.h"
#include"constante.h"
matrice* compute_score(matrice* plan, int** actif, int size);
int filtre_mat(matrice* input, int** actif, int size);
int pretty_mat(matrice* input, int** actif, int size);