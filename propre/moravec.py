import cv2 as cv
import numpy as np
import matplotlib.pyplot as plt
import os
import argparse

# Fonction Moravec pour la détection de points d'intérêt
def moravec(image, window_size=3):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    height, width = gray.shape
    points = []

    # Calcul du score de Moravec pour chaque pixel
    for y in range(window_size, height - window_size):
        for x in range(window_size, width - window_size):
            min_score = float('inf')
            for dy in range(-window_size, window_size + 1):
                for dx in range(-window_size, window_size + 1):
                    if dx == 0 and dy == 0:
                        continue
                    shifted = np.roll(gray, (dy, dx), axis=(0, 1))
                    score = np.sum((gray[y-window_size:y+window_size+1, x-window_size:x+window_size+1] - shifted[y-window_size:y+window_size+1, x-window_size:x+window_size+1])**2)
                    min_score = min(min_score, score)
            if min_score < 100000:
                points.append((x, y))

    return points

# Fonction pour correspondre les points et appliquer RANSAC
def ransac_match(points1, points2, threshold=2.0):
    points1 = np.array(points1)
    points2 = np.array(points2)

    # Appliquer RANSAC pour estimer une transformation affine entre les deux ensembles de points
    H, status = cv.findHomography(points1, points2, cv.RANSAC, threshold)
    
    return H, status

# Fonction principale pour charger les images, détecter les points et faire l'appariement
def process_images(image_name1, image_name2):
    # Créer les chemins complets des images dans le dossier 'points/images'
    img1_path = os.path.join('points/images', image_name1)
    img2_path = os.path.join('points/images', image_name2)
    
    # Charger les images
    img1 = cv.imread(img1_path)
    img2 = cv.imread(img2_path)

    # Détecter les points d'intérêt à l'aide de Moravec
    points1 = moravec(img1)
    points2 = moravec(img2)

    # Appliquer RANSAC pour trouver les correspondances entre les deux ensembles de points
    H, status = ransac_match(points1, points2)

    # Affichage des deux images avec les correspondances
    img1_show = img1.copy()
    img2_show = img2.copy()

    # Affichage des points détectés sur les deux images
    for pt in points1:
        cv.circle(img1_show, pt, 5, (0, 255, 0), -1)

    for pt in points2:
        cv.circle(img2_show, pt, 5, (0, 255, 0), -1)

    # Affichage des correspondances avec les lignes
    for i in range(len(points1)):
        if status[i]:
            pt1 = points1[i]
            pt2 = points2[i]
            cv.line(img1_show, pt1, pt2, (0, 255, 0), 1)
            cv.line(img2_show, pt2, pt1, (0, 255, 0), 1)

    # Affichage des images avec les correspondances
    plt.figure(figsize=(10, 5))
    plt.subplot(1, 2, 1)
    plt.imshow(cv.cvtColor(img1_show, cv.COLOR_BGR2RGB))
    plt.title("Image 1 avec points")

    plt.subplot(1, 2, 2)
    plt.imshow(cv.cvtColor(img2_show, cv.COLOR_BGR2RGB))
    plt.title("Image 2 avec points")

    plt.show()

# Point d'entrée du programme
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Appariement de points entre deux images avec Moravec et RANSAC.")
    parser.add_argument("image_name1", type=str, help="Nom de la première image (dans le dossier /points/images)")
    parser.add_argument("image_name2", type=str, help="Nom de la deuxième image (dans le dossier /points/images)")
    
    # Lecture des arguments
    args = parser.parse_args()
    
    # Appel de la fonction principale
    process_images(args.image_name1, args.image_name2)
