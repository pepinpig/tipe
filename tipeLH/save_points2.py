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
    np.savetxt(filename, points, fmt='%d')

# Créer un fichier d'en-tête avec le nombre de points
def save_point_count_header(filename, count1, count2):
    with open(filename, 'w') as f:
        f.write(f"#pragma once\n#define N {count1}\n")


def is_valid_refined_point(refined_corners):
    # Vérifie si un coin raffiné a été détecté (pas de None ou d'array vide)
    return refined_corners is not None and refined_corners.size > 0

# Raffinement des coins autour d'un point cliqué
def refine_corner(image, x, y):
    gray_image = cv.cvtColor(image, cv.COLOR_BGR2GRAY) if len(image.shape) == 3 else image
    corners = np.array([[[x, y]]], dtype=np.float32)
    criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

    # Taille de la fenêtre et zone zéro pour le raffinement
    window_size = (5, 5) 
    zero_zone = (-1, -1) 

    # Raffiner les coins
    refined_corners = cv.cornerSubPix(gray_image, corners, window_size, zero_zone, criteria)
    # Vérifie si le raffinement a réussi
    # Vérifie si le raffinement a réussi (en comparant les coordonnées)
    if refined_corners is not None and refined_corners.size > 0:
        refined_x, refined_y = refined_corners[0, 0]
        
        # Vérification si les coordonnées ont changé (raffinement effectué)
        if refined_x != x or refined_y != y:
            return refined_x, refined_y
        else:
            return None  # Pas de changement dans les coordonnées
    else:
        return None  # Pas de coin détecté

# Sélection des points pour la première image
def select_points_img1(event, x, y, flags, param):
   if event == cv.EVENT_LBUTTONDOWN:
        print("Point cliqué :")
        print(f"Original: x={x}, y={y}")
        
        # Essayer de raffiner le coin
        refined_point = refine_corner(param, x, y)
        
        if refined_point is not None:
            refined_x, refined_y = refined_point
            print(f"Raffiné: x={refined_x}, y={refined_y}")

            # Vérification si les coordonnées raffinées sont différentes des coordonnées initiales
            if abs(refined_x - x) > 1 or abs(refined_y - y) > 1:
                # Ajouter les coordonnées raffinées et afficher un cercle
                points_img1.append([refined_x, refined_y])
                refined_x_int, refined_y_int = int(refined_x), int(refined_y)
                cv.circle(param, (refined_x_int, refined_y_int), 5, (0, 255, 0), -1)
                cv.imshow('Image 1', param)
            else:
                print("Le raffinement n'a pas modifié les coordonnées. Essayez un autre point.")
        else:
            print("Aucun coin raffiné détecté. Veuillez cliquer sur un autre point.")

# Sélection des points pour la deuxième image
def select_points_img2(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        print("Point cliqué :")
        print(f"Original: x={x}, y={y}")
        
        # Essayer de raffiner le coin
        refined_point = refine_corner(param, x, y)
        
        if refined_point is not None:
            refined_x, refined_y = refined_point
            print(f"Raffiné: x={refined_x}, y={refined_y}")

            # Vérification si les coordonnées raffinées sont différentes des coordonnées initiales
            if abs(refined_x - x) > 1 or abs(refined_y - y) > 1:
                # Ajouter les coordonnées raffinées et afficher un cercle
                points_img2.append([refined_x, refined_y])
                refined_x_int, refined_y_int = int(refined_x), int(refined_y)
                cv.circle(param, (refined_x_int, refined_y_int), 5, (0, 255, 0), -1)
                cv.imshow('Image 2', param)
            else:
                print("Le raffinement n'a pas modifié les coordonnées. Essayez un autre point.")
        else:
            print("Aucun coin raffiné détecté. Veuillez cliquer sur un autre point.")

# Fonction principale pour charger les images et sélectionner les points
def process_images(image_name1, image_name2, calibrage):
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
    suffix = "calibrage_" if calibrage else ""

    save_points(f'points/donnees/points_{suffix}{base_name1}.txt', points_img1)
    save_points(f'points/donnees/points_{suffix}{base_name2}.txt', points_img2)

    # Créer le fichier d'en-tête
    save_point_count_header('N.h', len(points_img1), len(points_img2))
    print(f"Fichier d'en-tête créé avec N = {len(points_img1)}")

# Point d'entrée du programme
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sélectionner des points sur deux images.")
    parser.add_argument("image_name1", type=str, help="Nom de la première image.")
    parser.add_argument("image_name2", type=str, help="Nom de la deuxième image.")
    parser.add_argument("calibrage", type=int, help="1 pour le calibrage, 0 sinon.")
    args = parser.parse_args()

    print("- Cliquez pour sélectionner un point.")
    print("- Appuyez sur une touche pour terminer la sélection.")
    process_images(args.image_name1, args.image_name2, args.calibrage)
