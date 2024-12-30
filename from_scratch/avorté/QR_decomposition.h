#pragma once

// Copie une colonne de la matrice source dans une matrice destination
void copie_colonne(matrice* dest, matrice source, int colonne_source, int colonne_dest);

// Effectue la décomposition QR de la matrice A en matrices Q et R
void decomposition_QR(matrice A, matrice* Q, matrice* R);

// Algorithme QR pour calculer les valeurs propres et vecteurs propres
void QR_algorithm(matrice A, matrice* valeurs_propres, matrice* vecteurs_propres);