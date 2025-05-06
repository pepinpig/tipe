import numpy as np
import cv2

def svd_opencv_comparaison(mat_c):
    A = np.array(mat_c, dtype=np.float64)

    # OpenCV retourne: W (1D), U, Vt
    W, U, Vt = cv2.SVDecomp(A, flags=cv2.SVD_FULL_UV)

    # Créer une matrice S à partir de W
    S_matrix = np.zeros_like(A)
    np.fill_diagonal(S_matrix, W.flatten())

    # Reconstruction
    A_reconstruit = U @ S_matrix @ Vt

    print("Matrice A (originale):\n", A)
    print("Matrice U (OpenCV):\n", U)
    print("Matrice S (diagonale):\n", S_matrix)
    print("Matrice V^T (OpenCV):\n", Vt)
    print("A reconstruit (U*S*V^T):\n", A_reconstruit)

    erreur = np.linalg.norm(A - A_reconstruit)
    print(f"Erreur de reconstruction (norme Frobenius) : {erreur:.6e}")

    print("U^T * U ≈ I ?", np.allclose(U.T @ U, np.eye(U.shape[1]), atol=1e-6))
    print("V * V^T ≈ I ?", np.allclose(Vt.T @ Vt, np.eye(Vt.shape[1]), atol=1e-6))

# Exemple
matrice_test = [
    [-1, 0, 2, 0],
    [0, -1, 3, 8],
    [-8, 0, 0.1, 0.5],
    [9, -1, 0.45, 0]
]


svd_opencv_comparaison(matrice_test)
