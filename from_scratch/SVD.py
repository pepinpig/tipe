import numpy as np
import sys

def main():
    # Récupérer les fichiers d'entrée et de sortie
    input_file = sys.argv[1]
    U_file = sys.argv[2]
    S_file = sys.argv[3]
    V_file = sys.argv[4]

    # Lire la matrice A depuis le fichier
    A = np.loadtxt(input_file)

    # Calcul de la décomposition SVD
    U, S, Vt = np.linalg.svd(A, full_matrices=False)

    # Sauvegarder U, S et V dans des fichiers
    np.savetxt(U_file, U, fmt='%.18f')
    np.savetxt(S_file, np.diag(S), fmt='%.18f')  # La matrice S est une matrice diagonale
    np.savetxt(V_file, Vt, fmt='%.18f')

if __name__ == "__main__":
    main()
