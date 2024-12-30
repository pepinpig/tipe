import numpy as np

# Définir la taille N
N = 6

# Fonction pour charger les points d'image
def load_points_image(file_name):
    points = np.loadtxt(file_name)
    return points

# Fonction pour charger les points réels
def load_points_real(file_name):
    points = np.loadtxt(file_name)
    return points


# Fonction pour lire la matrice de projection P à partir du fichier
def load_projection_matrix(file_name):
    # Lire les dimensions de la matrice P
    with open(file_name, 'r') as f:
        rows, cols = map(int, f.readline().split())  # Lire les dimensions 3 x 4
        P = np.loadtxt(f)  # Lire les éléments de la matrice
    return P

def solve_library(A, Y):
    # Résoudre le système A * X = Y avec lstsq
    X, residuals, rank, s = np.linalg.lstsq(A, Y, rcond=None)
    return X


def solve_with_svd(A, Y):
    # Dimensions de la matrice A
    n, m = A.shape

    # Effectuer la décomposition SVD
    U, S, Vt = np.linalg.svd(A)

    # Créer la matrice S de taille n * m
    S_full = np.zeros((n, m))  # Matrice S de taille n x m
    np.fill_diagonal(S_full, S)  # Remplir la diagonale avec les valeurs singulières

    # Calcul de Σ^+ (pseudo-inverse de la matrice S)
    S_plus = np.zeros((m, n))  # S+ sera de taille m x n
    for i in range(min(n, m)):
        if abs(S[i]) > 1e-10:  # Vérifier les valeurs singulières non nulles
            S_plus[i, i] = 1.0 / S[i]
    # Calcul de A^+ = V * Σ^+ * U^T
    A_plus = np.dot(np.dot(Vt.T, S_plus), U.T)

    # Calcul de la solution X = A^+ * Y
    solution = np.dot(A_plus, Y)

    return solution


# Fonction principale
def main(image_name):
    # Initialisation des tableaux pour les points
    X = np.zeros(N)
    Y = np.zeros(N)
    Z = np.zeros(N)
    u = np.zeros(N)
    v = np.zeros(N)

    # Construction des noms de fichiers
    points_image_file = f"points/points_{image_name}.txt"
    points_reel_file = f"points/points_reel{image_name}.txt"
    p_file = f"P-{image_name}.txt"
    output_file = f"points/points_reconstruit{image_name}.txt"

    print("Lecture des points ...")
    u, v = load_points_image(points_image_file).T
    X, Y, Z = load_points_real(points_reel_file).T

    print("Lecture de P ...")
    P = load_projection_matrix(p_file)

    print("Reconstruction ...")

    # Ouvrir le fichier de sortie
    with open(output_file, "w") as output:
        for i in range(N):
            point_image = np.array([u[i], v[i], 1.0])  # Coordonnée homogène du point d'image
            print(f"Calcul pour le point {i} : ")
            print(point_image)

            # Résoudre pour X (les coordonnées réelles)
            result = solve_library(P, point_image)

            # Normalisation des coordonnées en divisant par la dernière composante (coordonnée homogène)
            result /= result[3]

            print(f"Coordonnées : {result[0]} {result[1]} {result[2]}")
            # Écriture des coordonnées dans le fichier
            output.write(f"{result[0]} {result[1]} {result[2]}\n")

    print(f"Coordonnées reconstruites écrites dans : {output_file}")

if __name__ == "__main__":
    # Exemple d'utilisation avec le nom de l'image passé en argument
    image_name = "cube"  # Remplacer par le nom de l'image voulu
    main(image_name)
