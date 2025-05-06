#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "manipulation_fichier.h"

#define precision 1e-18

// Fonction pour calculer la norme d'un vecteur colonne
double norme_vecteur_colonne(matrice a, int colonne);

int verifier_orthogonalite(matrice* M);

void decomposition_QR_householder(matrice* A, matrice* Q, matrice* R) ;

// Décomposition QR (Gram-Schmidt)
void decomposition_QR(matrice* A, matrice* Q, matrice* R);


void qr_algorithm(matrice *A, matrice* S);

// Décomposition en valeurs singulières(SVD)
void qr_algorithm_SVD(matrice *A, matrice* S, matrice* U, matrice* V);

//Cherche la valeur singuliere minimal (parcours la digonale de S en partant du bas et prend le premier element non nul)
double min_eig(matrice* A);

