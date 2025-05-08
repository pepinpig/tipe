import numpy as np
import cv2
import os
import sys
import subprocess

def correct_projection_matrix_YZ(P):
    """
    Applique une transformation inverse pour compenser l'échange Y <-> Z fait dans les points 3D.
    """
    T = np.array([
        [1, 0, 0, 0],
        [0, 0, 1, 0],
        [0, 1, 0, 0],
        [0, 0, 0, 1]
    ])
    return P @ T
def save_projection_matrix(P, image_name):
    """
    Sauvegarde la matrice P dans un fichier texte.
    """
    filename = f"P-{image_name}.txt"
    np.savetxt(filename, P, fmt="%.10f")
    print(f"Matrice de projection sauvegardée dans {filename}")


def load_points_2d(filename):
    """Charge les points 2D depuis un fichier, en ignorant les lignes (-1, -1)."""
    if not os.path.exists(filename):
        print(f"Erreur : fichier 2D introuvable: {filename}")
        sys.exit(1)

    points = []
    with open(filename, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            x, y = map(float, line.strip().split())
            if x == -1 and y == -1:
                continue
            points.append([x, y])
    return np.array(points, dtype=np.float32)

def load_points_3d(filename, swap_yz=False):
    """Charge les points 3D depuis un fichier. Peut échanger Y et Z si nécessaire."""
    if not os.path.exists(filename):
        print(f"Erreur : fichier 3D introuvable: {filename}")
        sys.exit(1)

    points = []
    with open(filename, 'r') as f:
        for line in f:
            if not line.strip():
                continue
            x, y, z = map(float, line.strip().split())
            if swap_yz:
                y, z = z, y
            points.append([x, y, z])
    return np.array(points, dtype=np.float32)

def compute_projection_matrix_autocalibration(object_points, image_points, image_size):
    """
    Utilise cv2.calibrateCamera pour estimer K, R, t, et calcule P = K [R|t]
    """
    object_points = [object_points]
    image_points = [image_points]

    ret, K, dist_coeffs, rvecs, tvecs = cv2.calibrateCamera(
        object_points, image_points, image_size, None, None
    )
    if not ret:
        raise RuntimeError("Échec de calibrateCamera")

    R, _ = cv2.Rodrigues(rvecs[0])
    t = tvecs[0]
    Rt = np.hstack((R, t))
    P = K @ Rt
    return P, K, R, t

def main():
    if len(sys.argv) > 2:
        image_name = sys.argv[1]
        classe = sys.argv[2]
        subprocess.run(["python3", "camera_calibration_save_points.py", f"{image_name}.jpg"])
    else:
        print("Lecture des fichiers existants...")
        image_name = "image_test"
        classe = "test"

    image_path = f"points/images/{image_name}.jpg"
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    if image is None:
        print(f"Erreur : image introuvable à {image_path}")
        sys.exit(1)
    image_size = (image.shape[1], image.shape[0])

    points_image_file = f"points/donnees/points_calibrage_{image_name}.txt"
    points_reel_file = f"points/donnees/points_reels_{classe}.txt"

    image_points = load_points_2d(points_image_file)
    object_points = load_points_3d(points_reel_file, swap_yz=True)  # active l'inversion Y-Z ici

    if image_points.shape[0] != object_points.shape[0]:
        print(f"Erreur : {image_points.shape[0]} points 2D et {object_points.shape[0]} points 3D.")
        sys.exit(1)

    P, K, R, t = compute_projection_matrix_autocalibration(object_points, image_points, image_size)

    print("Matrice intrinsèque K :\n", K)
    print("Rotation R :\n", R)
    print("Translation t :\n", t)
    P_corrected = correct_projection_matrix_YZ(P)
    print("Matrice de projection P (corrigée repère initial) :\n", P_corrected)
    save_projection_matrix(P_corrected, image_name)

if __name__ == "__main__":
    main()
