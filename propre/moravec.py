import cv2
import numpy as np
import matplotlib.pyplot as plt
import argparse
import matplotlib
matplotlib.use('TkAgg') 

def moravec_corner_detection(image, threshold=100):
    """
    Détecte les points d’intérêt en utilisant l’algorithme de Moravec.

    :param image: Image en niveaux de gris
    :param threshold: Seuil pour considérer un coin comme un point d'intérêt
    :return: Liste des coordonnées des points d'intérêt [(x1, y1), (x2, y2), ...]
    """
    # Dimensions de l’image
    height, width = image.shape

    # Déplacer la fenêtre de 1 pixel dans toutes les directions
    offsets = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]
    corner_response = np.zeros_like(image, dtype=np.float32)

    for y in range(1, height - 1):
        for x in range(1, width - 1):
            # Convertir le pixel en flottant pour éviter les débordements
            original = float(image[y, x])
            min_variance = float('inf')

            # Calculer les variations d’intensité pour chaque déplacement
            for dx, dy in offsets:
                shifted = float(image[y + dy, x + dx])
                variance = (original - shifted) ** 2
                min_variance = min(min_variance, variance)

            corner_response[y, x] = min_variance

    # Appliquer un seuil pour détecter les coins
    corners = np.argwhere(corner_response > threshold)

    return corners

def display_corners(image, corners):
    """
    Affiche les coins détectés sur l'image originale.
    
    :param image: Image originale
    :param corners: Liste des coordonnées des coins
    """
    plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    for corner in corners:
        y, x = corner
        plt.plot(x, y, 'ro', markersize=2)
    plt.title("Moravec Corner Detection")
    plt.axis('off')
    plt.show()

def main(image_name):
    # Chemin de l'image
    image_path = f'points/images/{image_name}'

    # Charger l'image en niveaux de gris
    image_gray = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    if image_gray is None:
        print(f"Erreur : Impossible de charger l'image {image_path}")
        return

    # Charger l'image en couleur pour l'affichage
    image_color = cv2.imread(image_path)

    # Détection des points d'intérêt
    corners = moravec_corner_detection(image_gray)

    # Afficher les coins détectés
    display_corners(image_color, corners)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Détection de points d'intérêt selon Moravec.")
    parser.add_argument("image_name", type=str, help="Nom de l'image (dans le dossier points/images/)")
    args = parser.parse_args()

    main(args.image_name)
