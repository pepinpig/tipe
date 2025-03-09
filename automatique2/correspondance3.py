import cv2 as cv
import numpy as np
import os
import sys

def load_matrix_f(filename):
    """Charge la matrice fondamentale F depuis un fichier."""
    return np.loadtxt(filename)

def load_points(filename):
    """Charge les points depuis un fichier texte."""
    return np.loadtxt(filename, dtype=int)

def save_epilines(filename, epilines):
    """Enregistre les droites épipolaires dans un fichier texte."""
    np.savetxt(filename, epilines, fmt='%f')

def compute_epilines(points, F, which_image):
    """Calcule les droites épipolaires pour les points donnés en utilisant la matrice fondamentale F."""
    lines = cv.computeCorrespondEpilines(points.reshape(-1, 1, 2), which_image, F)
    return lines.reshape(-1, 3)  # Renvoie une liste de triplets [a, b, c] pour ax + by + c = 0

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python script.py <image1> <image2> <points_file>")
        sys.exit(1)
    
    img1_path = sys.argv[1]
    img2_path = sys.argv[2]
    points_file = sys.argv[3]
    
    # Charger les images
    img1_name = os.path.splitext(os.path.basename(img1_path))[0]
    
    # Construire le chemin du fichier F
    F_filename = f"./points/donnees/F_{img1_name}.txt"
    
    if not os.path.exists(F_filename):
        print(f"Erreur: Fichier {F_filename} introuvable.")
        sys.exit(1)
    
    # Charger la matrice fondamentale
    F = load_matrix_f(F_filename)
    
    # Charger les points
    if not os.path.exists(points_file):
        print(f"Erreur: Fichier {points_file} introuvable.")
        sys.exit(1)
    points = load_points(points_file)
    
    # Calculer les droites épipolaires
    epilines = compute_epilines(points, F, 1)  # 1 = points de la première image
    
    # Sauvegarder les droites épipolaires
    epilines_filename = f"./points/donnees/epilines_{img1_name}.txt"
    save_epilines(epilines_filename, epilines)
    
    print(f"Droites épipolaires enregistrées dans {epilines_filename}.")
