import numpy as np
from scipy.linalg import svd, inv, qr
import numpy as np

def read_matrix_from_file(filename):
    try:
        with open(filename, 'r') as file:
            matrix = []
            next(file)
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

# Test de la fonction
def test_SVD():
    A=read_matrix_from_file("A-cube.txt")
    # Appeler la fonction
    U, S, Vt = svd(A)

    # Résultats
    print("Matrice K :")
    print(U)
    print("Matrice R :")
    print(S)
    print("Matrice T :")
    print(Vt)
    # Afficher les résultats
    print("Matrice de projection P :")
    print(P)

# Exécuter le test
test_SVD()
