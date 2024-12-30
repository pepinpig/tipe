#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrices.h"  // Votre fichier d'en-tête contenant la structure matrice et les fonctions

// Fonction prototype pour calculer f(a, b)
matrice f(const matrice* a, const matrice* b);

// Fonction prototype pour calculer la Jacobienne J
void jacobienne(const matrice* a, const matrice* b, matrice* J);

// Fonction principale pour résoudre un système non linéaire
void resolution_systeme_nl(matrice* a, const matrice* b) {
    matrice J = matrice_nulle(a->n, a->m);
    matrice delta_a = matrice_nulle(a->n, 1);
    matrice identite = matrice_identite(a->n);
    float lambda;

    // Calcul initial de lambda
    jacobienne(a, b, &J);
    matrice Jt = transposee(J);
    matrice JtJ = produit(Jt, J);
    lambda = 0.001 * diag_average(JtJ);

    for (int iter = 0; iter < 100; iter++) {
        jacobienne(a, b, &J);
        int compteur = 0;
        int accepte = 0;

        do {
            // Calcul de (J^T J + λI)
            matrice JtJ_lambdaI = somme(JtJ, multiplication_scalaire(identite, lambda));

            // Calcul de -J^T f(a, b)
            matrice fa = f(a, b);
            matrice Jtf = produit(Jt, fa);
            multiplication_scalaire(Jtf, -1);

            // Résolution de l'équation (J^T J + λI) ∆a = -J^T f(a, b)
            matrice* delta_a = resolution_systeme(&JtJ_lambdaI, &Jtf);

            // Vérifier si l'incrément diminue la fonction
            matrice a_new = somme(*a, *delta_a);
            matrice fa_new = f(&a_new, b);

            if (norme_vecteur(fa_new, 0) < norme_vecteur(fa, 0)) {
                *a = somme(*a, *delta_a);  // Mise à jour de a
                lambda /= 10;
                accepte = 1;
            } else {
                lambda *= 10;
            }

            compteur++;
            if (compteur > 100) break;  // Si trop d'itérations, sortir

            free_matrice(fa_new);
            free_matrice(a_new);
        } while (!accepte);
    }

    // Libération de la mémoire
    free_matrice(J);
    free_matrice(delta_a);
    free_matrice(identite);
    free_matrice(Jt);
    free_matrice(JtJ);
}

int main() {
    int n = 2, m = 2;
    matrice a = matrice_nulle(n, 1); // Valeurs initiales
    matrice b = matrice_nulle(m, 1); // Constantes

    // Initialiser a et b ici...

    resolution_systeme_nl(&a, &b);

    print_matrice(a);

    free_matrice(a);
    free_matrice(b);

    return 0;
}
