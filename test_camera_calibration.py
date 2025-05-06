import numpy as np
import cv2
import sys
import os

def load_points(file_path):
    """Charge les points depuis un fichier (ignore les -1 -1)."""
    points = []
    with open(file_path, 'r') as f:
        for line in f:
            if line.strip() == "":
                continue
            x, y = map(float, line.strip().split())
            if x == -1 and y == -1:
                continue
            points.append([x, y])
    return np.array(points)

def load_points_3d(file_path):
    """Charge les points 3D depuis un fichier."""
    points = []
    with open(file_path, 'r') as f:
        for line in f:
            if line.strip() == "":
                continue
            x, y, z = map(float, line.strip().split())
            points.append([x, y, z])
    return np.array(points)

def build_A_matrix(object_points, image_points):
    """Construit la matrice A pour le système AX = 0."""
    n = object_points.shape[0]
    A = []
    for i in range(n):
        X, Y, Z = object_points[i]
        u, v = image_points[i]
        row1 = [-X, -Y, -Z, -1, 0, 0, 0, 0, u*X, u*Y, u*Z, u]
        row2 = [0, 0, 0, 0, -X, -Y, -Z, -1, v*X, v*Y, v*Z, v]
        A.append(row1)
        A.append(row2)
    return np.array(A)

def compute_projection_matrix(A):
    """Calcule la matrice de projection P à partir de A via SVD."""
    _, _, VT = np.linalg.svd(A)
    P = VT[-1].reshape((3, 4))
    return P

def save_projection_matrix(P, image_name):
    """Sauvegarde la matrice P dans un fichier texte."""
    output_path = f"P-{image_name}.txt"
    np.savetxt(output_path, P, fmt="%.10f")
    print(f"Matrice de projection P sauvegardée dans {output_path}")

def main():
    if len(sys.argv) < 3:
        print("Usage: python3 camera_calibration_compute_P.py <image_name> <classe>")
        sys.exit(1)

    image_name = sys.argv[1]
    classe = sys.argv[2]

    points_2d_file = f"points/donnees/points_calibrage_{image_name}.txt"
    points_3d_file = f"points/donnees/points_reels_{classe}.txt"

    print("Chargement des points...")
    image_points = load_points(points_2d_file)
    object_points = load_points_3d(points_3d_file)

    if len(image_points) != len(object_points):
        print(f"Erreur : {len(image_points)} points 2D mais {len(object_points)} points 3D.")
        sys.exit(1)

    print("Construction de la matrice A...")
    A = build_A_matrix(object_points, image_points)

    print("Calcul de la matrice de projection P...")
    P = compute_projection_matrix(A)

    print("P =\n", P)
    save_projection_matrix(P, image_name)

if __name__ == "__main__":
    main()
