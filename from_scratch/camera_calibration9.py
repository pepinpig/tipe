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
def produit_matrice(A, B):
    # Utilise la méthode dot de NumPy pour le produit matriciel
    return np.dot(A, B)

def camera_calibration_resolution(A):
    # Étape 1 : Calcul de la décomposition SVD de A
    U, S, Vt = svd(A)

    # Trouver le vecteur singulier droit unitaire associé à la plus petite valeur singulière
    index_min = np.argmin(S)
    p = Vt[-1]  # Dernière colonne de V (ou dernière ligne de Vt)

    # Normalisation du vecteur p (composantes 9:11)
    norm = np.linalg.norm(p[8:11])
    p[8:11] /= norm

    # Étape 2 : Construire la matrice P
    P = np.zeros((3, 4))
    P[0, :] = p[:4]
    P[1, :] = p[4:8]
    P[2, :] = p[8:12]

    # Étape 3 : Calcul de la décomposition SVD de P
    U2, S2, Vt2 = svd(P)

    # Extraction du vecteur singulier droit associé à la plus petite valeur singulière
    C_approx = Vt2[-1]  # Dernière ligne de Vt2

    # Normalisation de C pour que le dernier élément soit 1
    C_approx /= C_approx[-1]

    # Étape 4 : QR factorisation de P(1:3, 1:3)^-1
    P_extract = P[:3, :3]
    P_inv = inv(P_extract)
    Q, R = qr(P_inv)

    # Étape 5 : Calcul de K, R et T
    K = inv(R)
    K /= K[2, 2]  # Normalisation pour que K[2,2] = 1

    R_inv = inv(Q)
    T = -K @ R_inv

    # Résultats
    print("Matrice K :")
    print(K)
    print("Matrice R :")
    print(R_inv)
    print("Matrice T :")
    print(T)

    return K, R_inv, T, P


def reconstruct_matrix_from_svd(U, S, Vt):
    """
    Reconstruit une matrice à partir des résultats SVD (U, S, Vt).
    Ajoute des zéros pour adapter S si A n'est pas carrée.
    """
    # Dimensions de la matrice originale
    m, n = U.shape[0], Vt.shape[1]
    
    # Crée une matrice S de dimensions compatibles avec U et Vt
    S_full = np.zeros((m, n))
    np.fill_diagonal(S_full, S)  # Place les valeurs singulières sur la diagonale
    
    # Reconstruction de la matrice originale
    return np.dot(U, np.dot(S_full, Vt))

def test_SVD():
    np.set_printoptions(precision=4, suppress=True)
    # Lecture de la matrice
    A = read_matrix_from_file("A-cube.txt")
    if A is None:
        return
    print("Matrice A :")
    print(A)  
    # Décomposition SVD
    U, S, Vt = svd(A)

    # Reconstruction de la matrice originale
    A_reconstruite = reconstruct_matrix_from_svd(U, S, Vt)

    # Afficher les résultats
    print("Matrice U :")
    print(U)
    print("Valeurs singulières (vecteur S) :")
    print(S)
    print("Matrice Vt :")
    print(Vt)
    print("Matrice reconstruite A :")
    print(A_reconstruite)

    # Vérifier si la reconstruction est correcte
    if np.allclose(A, A_reconstruite):
        print("La reconstruction est correcte : A ≈ U·S·Vt")
    else:
        print("La reconstruction est incorrecte.")

# Test de la fonction
def test_camera_calibration_resolution():
    A=read_matrix_from_file("A-cube.txt")

    # Appeler la fonction
    K, R, T, P = camera_calibration_resolution(A)

    # Afficher les résultats
    print("Matrice de projection P :")
    print(P)

# Exécuter le test
test_camera_calibration_resolution()
