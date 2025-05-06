import cv2 as cv
import os
import numpy as np
import argparse

# Liste pour stocker les points
points = []

# Redimensionne l’image sans dépasser les dimensions maximales
def resize_image(image, max_height=600, max_width=3000):
    height, width = image.shape[:2]
    height_ratio = max_height / height
    width_ratio = max_width / width
    resize_ratio = min(height_ratio, width_ratio, 1.0)
    new_width = int(width * resize_ratio)
    new_height = int(height * resize_ratio)
    return cv.resize(image, (new_width, new_height), interpolation=cv.INTER_AREA)

# Sauvegarde les points dans un fichier texte
def save_points(filename, points):
    with open(filename, 'w') as f:
        f.write(f"{len(points)} 2\n")
        np.savetxt(f, points, fmt='%d')

# Callback de sélection de points
def select_points(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        print(f"Point cliqué : x={x}, y={y}")
        points.append([x, y])
        cv.circle(param, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image', param)

# Fonction principale
def process_image(image_name):
    image_path = f'./points/images/{image_name}'
    original_img = cv.imread(image_path)

    if original_img is None:
        print("Erreur : l'image n'a pas pu être chargée.")
        return

    img = resize_image(original_img)

    cv.imshow('Image', img)
    cv.setMouseCallback('Image', select_points, param=img)

    print("Cliquez pour sélectionner les points. Appuyez sur une touche pour terminer.")
    cv.waitKey(0)

    base_name = os.path.splitext(image_name)[0]
    save_points(f'points/donnees/points_calibrage_{base_name}.txt', points)
    print(f"Points sauvegardés dans points_{base_name}.txt")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sélectionner des points sur une seule image.")
    parser.add_argument("image_name", type=str, help="Nom de l'image.")
    args = parser.parse_args()

    process_image(args.image_name)
