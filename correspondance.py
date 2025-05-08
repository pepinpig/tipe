import cv2 as cv
import os
import numpy as np
import sys

# Sauvegarder les points dans un fichier
def save_points(filename, points):
    np.savetxt(filename, points, fmt='%d')

# Charger les points depuis un fichier
def load_points(filename):
    if os.path.exists(filename):
        return np.loadtxt(filename, dtype=int).tolist()
    return []

# Sélection des points
def select_points(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        img = param['img']
        points = param['points']
        window_name = param['win']
        points.append([x, y])
        cv.circle(img, (x, y), 5, (0, 255, 0), -1)
        cv.imshow(window_name, img)

# Fonction pour dessiner les droites épipolaires
def draw_epilines(img, lines, pts):
    r, c, _ = img.shape
    for r_line, pt in zip(lines, pts):
        r_line = r_line[0]
        a, b, c_line = r_line
        if b != 0:
            x0, y0 = 0, int(-c_line / b)
            x1, y1 = img.shape[1], int(-(c_line + a * img.shape[1]) / b)
            cv.line(img, (x0, y0), (x1, y1), (0, 255, 0), 1)
            cv.circle(img, tuple(pt), 5, (255, 0, 0), -1)
    return img

# Afficher les droites épipolaires au clic
def on_click_show_epiline(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        F = param['F']
        if param['which'] == 'img1':
            point = np.array([[[x, y]]], dtype=np.float32)
            lines = cv.computeCorrespondEpilines(point, 1, F)
            print(lines)
            img2_with_lines = draw_epilines(param['img2'].copy(), lines, param['pts2'])
            cv.imshow('Image 2', img2_with_lines)
        elif param['which'] == 'img2':
            point = np.array([[[x, y]]], dtype=np.float32)
            lines = cv.computeCorrespondEpilines(point, 2, F)
            print(lines)
            print(F*l)
            img1_with_lines = draw_epilines(param['img1'].copy(), lines, param['pts1'])
            cv.imshow('Image 1', img1_with_lines)

def main(image1_name, image2_name):
    # Définition des chemins des fichiers
    image1_path = f"points/images/{image1_name}.jpg"
    image2_path = f"points/images/{image2_name}.jpg"
    points1_path = f"points/donnees/{image1_name}_points.txt"
    points2_path = f"points/donnees/{image2_name}_points.txt"

    # Charger les images
    img1 = cv.imread(image1_path)
    img2 = cv.imread(image2_path)

    if img1 is None or img2 is None:
        print("Erreur : l'une des images n'a pas pu être chargée.")
        sys.exit(1)

    # Charger les points sauvegardés
    points_img1 = load_points(points1_path)
    points_img2 = load_points(points2_path)

    # Si pas assez de points, les sélectionner manuellement
    if len(points_img1) < 8 or len(points_img2) < 8:
        cv.imshow('Image 1', img1)
        cv.imshow('Image 2', img2)
        cv.setMouseCallback('Image 1', select_points, param={'img': img1, 'points': points_img1, 'win': 'Image 1'})
        cv.setMouseCallback('Image 2', select_points, param={'img': img2, 'points': points_img2, 'win': 'Image 2'})

        print("Cliquez sur au moins 8 points correspondants dans chaque image, puis appuyez sur une touche.")
        cv.waitKey(0)
        cv.destroyAllWindows()

        save_points(points1_path, points_img1)
        save_points(points2_path, points_img2)

    # Recharger après sauvegarde
    points_img1 = load_points(points1_path)
    points_img2 = load_points(points2_path)

    if len(points_img1) >= 8 and len(points_img2) >= 8:
        pts1 = np.int32(points_img1)
        pts2 = np.int32(points_img2)

        print("Calcul de la matrice fondamentale...")
        F, mask = cv.findFundamentalMat(pts1, pts2, cv.FM_LMEDS)
        print("Matrice fondamentale :\n", F)
        f_path = f"points/donnees/F_{image1_name}.txt"
        np.savetxt(f_path, F)

        print("Cliquez sur un point pour afficher les droites épipolaires.")

        cv.imshow('Image 1', img1)
        cv.imshow('Image 2', img2)

        cv.setMouseCallback('Image 1', on_click_show_epiline, param={
            'which': 'img1',
            'img1': img1,
            'img2': img2,
            'F': F,
            'pts2': pts2,
            'pts1': pts1
        })

        cv.setMouseCallback('Image 2', on_click_show_epiline, param={
            'which': 'img2',
            'img1': img1,
            'img2': img2,
            'F': F,
            'pts2': pts2,
            'pts1': pts1
        })

        cv.waitKey(0)
        cv.destroyAllWindows()
    else:
        print("Pas assez de points (8 minimum requis).")

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python plot_points_ap.py <image1_name> <image2_name>")
        sys.exit(1)
    
    image1_name = sys.argv[1]
    image2_name = sys.argv[2]
    main(image1_name, image2_name)
