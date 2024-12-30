import cv2 as cv
import os
import numpy as np
import argparse

# Listes pour stocker les points correspondants
points_img1 = []
points_img2 = []

# Fonction pour redimensionner l'image en conservant le ratio et avec des dimensions maximales de 600px de hauteur et 3000px de largeur
def resize_image(image, max_height=600, max_width=3000):
    height, width = image.shape[:2]

    # Calcul du ratio de redimensionnement en fonction des limites de largeur et de hauteur
    height_ratio = max_height / height
    width_ratio = max_width / width
    resize_ratio = min(height_ratio, width_ratio, 1.0)  # Assure que l'image ne sera pas agrandie

    # Calcul des nouvelles dimensions
    new_width = int(width * resize_ratio)
    new_height = int(height * resize_ratio)

    return cv.resize(image, (new_width, new_height), interpolation=cv.INTER_AREA)

# Sauvegarder les points dans un fichier
def save_points(filename, points):
    np.savetxt(filename, points, fmt='%d')

# Créer le fichier N.h
def save_point_count_header(filename, count1, count2):
    with open(filename, 'w') as f:
        f.write(f"#pragma once\n#define N {count1}\n")

# Sélection des points pour la première image
def select_points_img1(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        points_img1.append([x, y])
        cv.circle(param, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image 1', param)

# Sélection des points pour la deuxième image
def select_points_img2(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        points_img2.append([x, y])
        cv.circle(param, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image 2', param)

# Fonction principale pour charger les images et sélectionner les points
def process_images(image_name1, image_name2):
    # Charger la première image
    image_path1 = f'./points/images/{image_name1}'
    original_img1 = cv.imread(image_path1)
    img1 = resize_image(original_img1)

    # Charger la deuxième image
    image_path2 = f'./points/images/{image_name2}'
    original_img2 = cv.imread(image_path2)
    img2 = resize_image(original_img2)

    # Afficher la première image et configurer les clics pour la sélection des points
    cv.imshow('Image 1', img1)
    cv.imshow('Image 2', img2)
    cv.setMouseCallback('Image 1', select_points_img1, param=img1)
    cv.setMouseCallback('Image 2', select_points_img2, param=img2)

    # Attendre la sélection des points pour la première image
    print("Sélectionnez les points pour la première image et appuyez sur une touche pour continuer.")
    cv.waitKey(0)

    cv.destroyWindow('Image 1')
    cv.destroyWindow('Image 2')

    # Sauvegarder les points pour la première image
    base_name1 = os.path.splitext(image_name1)[0]
    output_filename1 = f'points/donnees/points_{base_name1}.txt'
    save_points(output_filename1, points_img1)
    print(f'Points sauvegardés pour la première image dans le fichier : {output_filename1}')

    # Sauvegarder les points pour la deuxième image
    base_name2 = os.path.splitext(image_name2)[0]
    output_filename2 = f'points/donnees/points_{base_name2}.txt'
    save_points(output_filename2, points_img2)
    print(f'Points sauvegardés pour la deuxième image dans le fichier : {output_filename2}')

    # Créer le fichier N.h avec le nombre de points sélectionnés pour les deux images
    header_filename = f'N.h'
    save_point_count_header(header_filename, len(points_img1), len(points_img2))
    print(f"Fichier d'en-tête créé : {header_filename} avec N1 = {len(points_img1)} et N2 = {len(points_img2)}")

# Point d'entrée du programme
if __name__ == "__main__":
    # Création d'un parser pour les arguments de la ligne de commande
    parser = argparse.ArgumentParser(description="Sélectionner des points sur deux images et les sauvegarder dans des fichiers texte.")
    parser.add_argument("image_name1", type=str, help="Nom de la première image (dans le dossier /images)")
    parser.add_argument("image_name2", type=str, help="Nom de la deuxième image (dans le dossier /images)")

    # Lecture des arguments
    args = parser.parse_args()

    # Appel de la fonction principale
    process_images(args.image_name1, args.image_name2)
