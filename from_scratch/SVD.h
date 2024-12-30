#pragma once

// Fonction pour calculer la norme d'un vecteur colonne
long double norme_vecteur_colonne(matrice a, int colonne);

// Décomposition QR (Gram-Schmidt)
void decomposition_QR(matrice* A, matrice* Q, matrice* R);

// Décomposition RQ
void decomposition_RQ(matrice* A, matrice* Q, matrice* R);
void qr_algorithm(matrice *A, matrice* S);
void qr_algorithm_SVD(matrice *A, matrice* S, matrice* U, matrice* V);
// Décomposition en valeurs singulières approximative (SVD)
/*void decomposition_SVD_approche(matrice* A, matrice* U, matrice* S, matrice* V);*/

//Cherche la valeur singuliere minimal (parcours la digonale de S en partant du bas et prend le premier element non nul)
long double min_eig(matrice A);
int test();
