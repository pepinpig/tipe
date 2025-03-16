#ifndef SVD_H
#define SVD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "manipulation_fichier.h"

#define precision 1e-18

// Fonction pour calculer la norme d'un vecteur colonne
long double norme_vecteur_colonne(matrice a, int colonne);

// Décomposition QR (Gram-Schmidt)
void decomposition_QR(matrice* A, matrice* Q, matrice* R);

// Décomposition RQ
void decomposition_RQ(matrice* A, matrice* Q, matrice* R);
void qr_algorithm(matrice *A, matrice* S);
void qr_algorithm_SVD(matrice *A, matrice* S, matrice* U, matrice* V);
void python_SVD(matrice *A, matrice* S, matrice* U, matrice* V);

// Décomposition en valeurs singulières approximative (SVD)
/*void decomposition_SVD_approche(matrice* A, matrice* U, matrice* S, matrice* V);*/
matrice resolution_systeme_SVD(matrice* A, matrice* Y);

//Cherche la valeur singuliere minimal (parcours la digonale de S en partant du bas et prend le premier element non nul)
long double min_eig(matrice A);
int test();
#endif
