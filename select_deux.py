import cv2 as cv
import os
import numpy as np
import argparse

# Listes pour stocker les points correspondants
points_img1 = []
points_img2 = []

# Fonction pour redimensionner l'image en conservant le ratio et avec des dimensions maximales
def resize_image(image, max_height=600, max_width=3000):
    height, width = image.shape[:2]
    height_ratio = max_height / height
    width_ratio = max_width / width
    resize_ratio = min(height_ratio, width_ratio, 1.0)  # Assure que l'image ne sera pas agrandie
    new_width = int(width * resize_ratio)
    new_height = int(height * resize_ratio)
    return cv.resize(image, (new_width, new_height), interpolation=cv.INTER_AREA)

# Sauvegarder les points dans un fichier
def save_points(filename, points):
    with open(filename, 'w') as f:
        f.write(f"{len(points)} 2\n")  # Écrit le nombre de points en première ligne
        np.savetxt(f, points, fmt='%d')

# Sélection des points pour la première image
def select_points_img1(event, x, y, flags, param):
   if event == cv.EVENT_LBUTTONDOWN:
        print("Point cliqué :")
        print(f"Original: x={x}, y={y}")
        points_img1.append([x, y])
        cv.circle(param, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image 1', param)

# Sélection des points pour la première image
def select_points_img2(event, x, y, flags, param):
   if event == cv.EVENT_LBUTTONDOWN:
        print("Point cliqué :")
        print(f"Original: x={x}, y={y}")
        points_img2.append([x, y])
        cv.circle(param, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image 2', param)

# Fonction principale pour charger les images et sélectionner les points
def process_images(image_name1, image_name2):
    # Charger les images
    image_path1 = f'./points/images/{image_name1}'
    image_path2 = f'./points/images/{image_name2}'
    original_img1 = cv.imread(image_path1)
    original_img2 = cv.imread(image_path2)

    # Vérification des images chargées
    if original_img1 is None or original_img2 is None:
        print("Erreur : Une ou plusieurs images n'ont pas pu être chargées.")
        return

    img1 = resize_image(original_img1)
    img2 = resize_image(original_img2)

    # Afficher les images et configurer les callbacks
    cv.imshow('Image 1', img1)
    cv.imshow('Image 2', img2)
    cv.setMouseCallback('Image 1', select_points_img1, param=img1)
    cv.setMouseCallback('Image 2', select_points_img2, param=img2)

    print("Sélectionnez les points pour les images. Appuyez sur une touche pour continuer.")
    cv.waitKey(0)

    # Sauvegarder les points sélectionnés
    base_name1 = os.path.splitext(image_name1)[0]
    base_name2 = os.path.splitext(image_name2)[0]
    save_points(f'points/donnees/points_{base_name1}.txt', points_img1)
    save_points(f'points/donnees/points_{base_name2}.txt', points_img2)
    print(f"Points sauvegardés dans points_{base_name1} et points_{base_name2}")

# Point d'entrée du programme
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sélectionner des points sur deux images.")
    parser.add_argument("image_name1", type=str, help="Nom de la première image.")
    parser.add_argument("image_name2", type=str, help="Nom de la deuxième image.")
    args = parser.parse_args()

    print("- Cliquez pour sélectionner un point.")
    print("- Appuyez sur une touche pour terminer la sélection.")
    process_images(args.image_name1, args.image_name2)
