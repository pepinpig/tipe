import numpy as np
import argparse

def save_matrix_to_file(matrix, name, image_name):
    filename = f"points/{name}-{image_name}.txt"
    np.savetxt(filename, matrix, fmt='%.6f')
    print(f"Matrix {name} saved to {filename}")

def lire_matrice(fichier):
    """Lit une matrice à partir d'un fichier texte."""
    with open(fichier, 'r') as f:
        matrice = np.loadtxt(f, dtype=float)
    return matrice

def construire_matrice_A(P1, P2, u1, v1, u2, v2):
    """Construit la matrice A pour un point donné."""
    A = np.zeros((4, 4))
    A[0] = u1 * P1[2] - P1[0]
    A[1] = v1 * P1[2] - P1[1]
    A[2] = u2 * P2[2] - P2[0]
    A[3] = v2 * P2[2] - P2[1]
    return A

def trianguler_points(P1, P2, points_image1, points_image2):
    """
    Triangule les points 3D à partir des correspondances dans deux images.
    """
    points_3D = []
    for (u1, v1), (u2, v2) in zip(points_image1, points_image2):
        A = construire_matrice_A(P1, P2, u1, v1, u2, v2)
        _, S, Vt = np.linalg.svd(A)
        X_homog = Vt[-1]
        X = X_homog[:3] / X_homog[3]  # Normalisation homogène
        points_3D.append(X)
    return np.array(points_3D)

def main():
    # Parse arguments
    parser = argparse.ArgumentParser(description="Triangulation 3D à partir de matrices de projection et de points.")
    parser.add_argument("nom_image1", help="Nom de la première image (sans extension).")
    parser.add_argument("nom_image2", help="Nom de la deuxième image (sans extension).")
    args = parser.parse_args()

    # Construire les chemins des fichiers
    fichier_P1 = f"P-{args.nom_image1}.txt"
    fichier_P2 = f"P-{args.nom_image2}.txt"
    fichier_points1 = f"points/points_{args.nom_image1}.txt"
    fichier_points2 = f"points/points_{args.nom_image2}.txt"

    # Lecture des matrices et des points
    P1 = lire_matrice(fichier_P1)
    P2 = lire_matrice(fichier_P2)
    points_image1 = lire_matrice(fichier_points1)
    print(points_image1)
    points_image2 = lire_matrice(fichier_points2)

    # Triangulation
    points_3D = trianguler_points(P1, P2, points_image1, points_image2)

    # Résultat
    print("Points 3D reconstruits :")
    print(points_3D)
    save_matrix_to_file(points_3D, "points_3D", args.nom_image1)

if __name__ == "__main__":
    main()
