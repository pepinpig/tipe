import numpy as np
import cv2
import sys

def read_points(filename):
    """Lit un fichier contenant des points 2D et ignore les lignes -1 -1."""
    points = []
    with open(filename, 'r') as f:
        for line in f:
            x, y = map(float, line.strip().split())
            if x != -1 and y != -1:
                points.append([x, y])
    return np.array(points).T  # OpenCV attend un tableau 2xN

def read_projection_matrix(filename):
    """Lit une matrice de projection 3x4 depuis un fichier texte."""
    with open(filename, 'r') as f:
        data = [list(map(float, line.strip().split())) for line in f]
    return np.array(data)

def main(img1, img2):
    # Chemins vers les fichiers
    file_pts1 = f"points/donnees/points_ap_{img1}.txt"
    file_pts2 = f"points/donnees/points_ap_{img2}.txt"
    file_P1 = f"points/donnees/P-{img1}.txt"
    file_P2 = f"points/donnees/P-{img2}.txt"
    output_file = f"points/donnees/points_3d_{img1}.txt"

    # Lecture des données
    pts1 = read_points(file_pts1)
    pts2 = read_points(file_pts2)
    if pts1.shape[1] != pts2.shape[1]:
        print("Erreur : le nombre de points valides n'est pas le même.")
        return

    P1 = read_projection_matrix(file_P1)
    P2 = read_projection_matrix(file_P2)

    # Triangulation
    points_4d = cv2.triangulatePoints(P1, P2, pts1, pts2)
    points_3d = points_4d[:3] / points_4d[3]

    # Sauvegarde
    np.savetxt(output_file, points_3d.T, fmt="%.6f")
    print(f"Points 3D écrits dans : {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: python3 {sys.argv[0]} <image1> <image2>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
