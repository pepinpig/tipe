import cv2 as cv
import os
import numpy as np

# Listes pour stocker les points correspondants
points_img1 = []
points_img2 = []

# Fonction pour redimensionner une image à la moitié de sa taille
def resize_image(image):
    width = int(image.shape[1] / 3)
    height = int(image.shape[0] / 3)
    return cv.resize(image, (width, height))

# Charger et redimensionner les images
img1 = resize_image(cv.imread('images/calibration/1.JPEG'))
img2 = resize_image(cv.imread('images/calibration/2.JPEG'))

# Sauvegarder les points dans un fichier
def save_points(filename, points):
    np.savetxt(filename, points, fmt='%d')


# Sélection des points
def select_points(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        # Sélectionner des points sur l'image 1
        if param == 'img1':
            points_img1.append([x, y])
            cv.circle(img1, (x, y), 5, (0, 255, 0), -1)
            cv.imshow('Image 1', img1)
        # Sélectionner des points sur l'image 2
        elif param == 'img2':
            points_img2.append([x, y])
            cv.circle(img2, (x, y), 5, (0, 255, 0), -1)
            cv.imshow('Image 2', img2)

# Afficher les images et configurer les clics pour la sélection des points
cv.imshow('Image 1', img1)
cv.imshow('Image 2', img2)

cv.setMouseCallback('Image 1', select_points, param='img1')
cv.setMouseCallback('Image 2', select_points, param='img2')
# Attendre la sélection des points
cv.waitKey(0)

    # Sauvegarder les points sélectionnés
save_points('points_img1.txt', points_img1)
save_points('points_img2.txt', points_img2)

