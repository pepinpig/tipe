import numpy as np
import scipy as sp

def read_matrix_from_file(filename):
    try:
        with open(filename, 'r') as file:
            matrix = []
            for line in file:
                row = list(map(float, line.strip().split()))
                matrix.append(row)
            return np.array(matrix)
    except FileNotFoundError:
        print(f"Erreur : Le fichier {filename} n'existe pas.")
        return None
    except ValueError:
        print("Erreur : Le fichier contient des valeurs non valides.")
        return None


def svd_decomposition(matrix):
    """
    Calcule la décomposition SVD d'une matrice.

    Args:
        matrix (ndarray): La matrice à décomposer.

    Returns:
        U (ndarray): Matrice unitaire gauche.
        S (ndarray): Valeurs singulières sous forme de vecteur.
        Vt (ndarray): Matrice unitaire droite transposée.
    """
    # Calcul de la SVD
    U, S, Vt = np.linalg.svd(matrix, full_matrices=False)

    return U, S, Vt

# Exemple d'utilisation
if __name__ == "__main__":
    A=np.array([[-1.,   0.,   0.,   0. ],
 [ 0.,  -1.,   0.,   0. ],
 [-1.,   0.,   0.1,  0.5],
 [ 0.,  -1.,   0.,   0. ]]
)
    At = A.T
    AAt = np.dot(A, At)
    Q, R = sp.linalg.qr(A)
    print("A decomp")
    print("Matrice Q :\n", Q)
    print("Matrice R :\n", R)
    print("A reconstruit :\n",np.dot(Q, R))
    # Calcul de la SVD
    U, S, Vt = svd_decomposition(A)

    # Affichage des résultats
    print("Matrice originale A :")
    print(A)
    print("\nMatrice U (unitaires gauche) :")
    print(U)
    print("\nValeurs singulières S :")
    print(S)
    print("\nMatrice V^T (unitaires droite transposée) :")
    print(Vt)

    # Reconstruction de la matrice originale à partir de la SVD
    A_reconstructed = np.dot(U, np.dot(np.diag(S), Vt))
    print("\nMatrice reconstruite (U * S * V^T) :")
    print(A_reconstructed)

