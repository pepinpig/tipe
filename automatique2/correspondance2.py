import cv2 as cv
import os
import numpy as np
import sys

# Vérifier les arguments
if len(sys.argv) != 3:
    print("Usage: python script.py <image1> <image2>")
    sys.exit(1)

img1_path = sys.argv[1]
img2_path = sys.argv[2]

# Extraire les noms de fichiers sans extension
img1_name = os.path.splitext(os.path.basename(img1_path))[0]
img2_name = os.path.splitext(os.path.basename(img2_path))[0]

# Charger les images
img1 = cv.imread(img1_path)
img2 = cv.imread(img2_path)

if img1 is None or img2 is None:
    print("Erreur: Impossible de charger les images.")
    sys.exit(1)

# Dossiers pour sauvegarder les points et la matrice fondamentale
points_dir = "./points/donnes"
os.makedirs(points_dir, exist_ok=True)
F_filename = os.path.join(points_dir, f"F_{img1_name}.txt")

def save_points(filename, points):
    np.savetxt(filename, points, fmt='%d')

def load_points(filename):
    if os.path.exists(filename):
        return np.loadtxt(filename, dtype=int).tolist()
    return []

points_img1 = load_points(f'points_{img1_name}.txt')
points_img2 = load_points(f'points_{img2_name}.txt')

def select_points(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        if param == 'img1':
            points_img1.append([x, y])
            cv.circle(img1, (x, y), 5, (0, 255, 0), -1)
            cv.imshow('Image 1', img1)
        elif param == 'img2':
            points_img2.append([x, y])
            cv.circle(img2, (x, y), 5, (0, 255, 0), -1)
            cv.imshow('Image 2', img2)

if len(points_img1) < 8 or len(points_img2) < 8:
    cv.imshow('Image 1', img1)
    cv.imshow('Image 2', img2)
    cv.setMouseCallback('Image 1', select_points, param='img1')
    cv.setMouseCallback('Image 2', select_points, param='img2')
    cv.waitKey(0)
    save_points(f'points_{img1_name}.txt', points_img1)
    save_points(f'points_{img2_name}.txt', points_img2)

points_img1 = load_points(f'points_{img1_name}.txt')
points_img2 = load_points(f'points_{img2_name}.txt')

if len(points_img1) >= 8 and len(points_img2) >= 8:
    pts1 = np.int32(points_img1)
    pts2 = np.int32(points_img2)
    
    F, mask = cv.findFundamentalMat(pts1, pts2, cv.FM_LMEDS)
    np.savetxt(F_filename, F, fmt='%f')
    print(f"Matrice fondamentale enregistrée dans {F_filename}")
else:
    print("Pas assez de points de correspondance sélectionnés (8 minimum).")