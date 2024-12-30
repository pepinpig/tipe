import cv2 as cv
import os
import numpy as np
import argparse

# Variables globales pour stocker les points et gérer l'état
current_point = []
all_points = []
images = []
image_windows = []
current_image_index = 0

# Fonction pour redimensionner l'image
def resize_image(image, max_height=600, max_width=3000):
    height, width = image.shape[:2]
    height_ratio = max_height / height
    width_ratio = max_width / width
    resize_ratio = min(height_ratio, width_ratio, 1.0)
    new_width = int(width * resize_ratio)
    new_height = int(height * resize_ratio)
    return cv.resize(image, (new_width, new_height), interpolation=cv.INTER_AREA)

# Sauvegarder les points dans un fichier
def save_points(filename, points):
    with open(filename, 'w') as f:
        for point_set in points:
            f.write(",".join(str(pt) if pt else "None" for pt in point_set) + "\n")

# Gestion des clics de souris pour sélectionner un point
def select_point(event, x, y, flags, param):
    global current_point
    if event == cv.EVENT_LBUTTONDOWN:
        current_point[current_image_index] = (x, y)
        img_with_point = param.copy()
        cv.circle(img_with_point, (x, y), 5, (0, 255, 0), -1)
        cv.imshow("Image", img_with_point)

# Fonction principale
def process_images(image_names):
    global current_point, all_points, images, current_image_index

    # Charger les images
    for image_name in image_names:
        image_path = f'./points/images/{image_name}'
        original_img = cv.imread(image_path)
        resized_img = resize_image(original_img)
        images.append(resized_img)

    print("Instructions :")
    print("- Cliquez pour sélectionner un point sur une image.")
    print("- Appuyez sur 'v' pour valider la sélection d'un point.")
    print("- Appuyez sur 'n' pour passer à l'image suivante sans sélectionner de point.")
    print("- Appuyez sur Entrée pour terminer la sélection.")

    # Initialisation
    num_images = len(images)
    while True:
        current_point = [None] * num_images  # Reset des points pour le tour
        for i in range(num_images):
            current_image_index = i
            cv.imshow("Image", images[i])
            cv.setMouseCallback("Image", select_point, param=images[i])

            # Attendre une action utilisateur
            while True:
                key = cv.waitKey(0)
                if key == 13:  # Entrée pour terminer
                    break  # Terminer la boucle principale
                elif key == ord('n'):  # Passer sans sélectionner
                    break
                elif key == ord('v') and current_point[current_image_index] is not None:  # Valider un point
                    break

            if key == 13:  # Si Entrée, terminer la boucle principale
                break

        # Ajouter le tour complet des points au tableau global
        all_points.append(current_point)

        # Si Entrée, terminer
        if key == 13:
            break

    # Sauvegarder les résultats
    output_filename = 'points/donnees/points.txt'
    save_points(output_filename, all_points)
    print(f"Tous les points ont été sauvegardés dans le fichier : {output_filename}")

    # Fermer toutes les fenêtres
    cv.destroyAllWindows()

# Point d'entrée du programme
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Sélectionner des points sur plusieurs images et les sauvegarder.")
    parser.add_argument("image_names", nargs='+', type=str, help="Noms des images (dans le dossier /images)")
    args = parser.parse_args()

    process_images(args.image_names)
