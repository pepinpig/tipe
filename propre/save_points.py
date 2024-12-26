import cv2 as cv
import os
import numpy as np
import argparse

# Listes pour stocker les points correspondants
points_img = []

# Fonction pour redimensionner l'image en conservant le ratio et avec des dimensions maximales de 1000px de hauteur et 3000px de largeur
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
def save_point_count_header(filename, count):
    with open(filename, 'w') as f:
        f.write(f"#pragma once\n#define N {count}\n")

# Sélection des points
def select_points(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        points_img.append([x, y])
        cv.circle(img, (x, y), 5, (0, 255, 0), -1)
        cv.imshow('Image', img)

# Fonction principale pour charger l'image et sélectionner les points
def process_image(image_name):
    global img
    image_path = f'./points/images/{image_name}'

    # Charger l'image et la redimensionner avec les dimensions maximales
    original_img = cv.imread(image_path)
    img = resize_image(original_img)
    
    # Afficher l'image et configurer les clics pour la sélection des points
    cv.imshow('Image', img)
    cv.setMouseCallback('Image', select_points)
    
    # Attendre la sélection des points
    cv.waitKey(0)
    
    # Extraire le nom de l'image sans l'extension
    base_name = os.path.splitext(image_name)[0]
    
    # Créer le nom du fichier texte pour sauvegarder les points
    output_filename = f'points/donnees/points_{base_name}.txt'
    
    # Sauvegarder les points sélectionnés
    save_points(output_filename, points_img)
    print(f'Points sauvegardés dans le fichier : {output_filename}')
    
    # Créer le fichier N.h avec le nombre de points sélectionnés
    header_filename = f'N.h'
    save_point_count_header(header_filename, len(points_img))
    print(f"Fichier d'en-tête créé : {header_filename} avec N = {len(points_img)}")

# Point d'entrée du programme
if __name__ == "__main__":
    # Création d'un parser pour les arguments de la ligne de commande
    parser = argparse.ArgumentParser(description="Sélectionner des points sur une image et les sauvegarder dans un fichier texte.")
    parser.add_argument("image_name", type=str, help="Nom de l'image (dans le dossier /images)")
    
    # Lecture des arguments
    args = parser.parse_args()
    
    # Appel de la fonction principale
    process_image(args.image_name)
