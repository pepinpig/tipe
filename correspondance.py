import cv2 as cv
import os
import numpy as np

# Listes pour stocker les points correspondants
points_img1 = []
points_img2 = []

# Charger les images
img1 = cv.imread('images/IMG_0078.jpg')
img2 = cv.imread('images/IMG_0079.jpg')

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

# Charger les points précédemment sauvegardés
points_img1 = load_points('points_img1.txt')
points_img2 = load_points('points_img2.txt')

# Si les points ne sont pas déjà sauvegardés, permettre la sélection par clic
if len(points_img1) < 8 or len(points_img2) < 8:
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

# Charger les points précédemment sauvegardés
points_img1 = load_points('points_img1.txt')
points_img2 = load_points('points_img2.txt')


# Vérifier si suffisamment de points sont sélectionnés
if len(points_img1) >= 8 and len(points_img2) >= 8:
    # Convertir les listes en arrays numpy
    pts1 = np.int32(points_img1)
    pts2 = np.int32(points_img2)

    # Calculer la matrice fondamentale F
    print("Calcul de la matrice...")
    F, mask = cv.findFundamentalMat(pts1, pts2, cv.FM_LMEDS) 
    '''mask est utilisé pour indiquer quels points ont été 
    considérés comme inliers (valeurs acceptées) lors 
    du calcul de la matrice fondamentale F'''
    #FM_LMEDS "Least Median of Squares" (méthode des moindres médianes des carrés).
    print("Matrice calculée !")


    # Fonction pour dessiner les droites épipolaires
    def draw_epilines(img, lines, pts):
        ''' Dessine les droites épipolaires sur l'image en fonction des points correspondants '''
        r, c, _ = img.shape #r:rows nb de ligne de l'image = hauteur
                            #c:columns nb de colonnes de l'image= largeur
        #itération sur tous les couples
        for l, pt in zip(lines, pts):
            r = l[0]
            x0, y0 = map(int, [0, -r[2] / r[1]])
            '''x0 est fixé à 0, ce qui représente le bord gauche de 
            l'image. y0 est calculé à l'aide de l'équation de la ligne epipolaire
            Lorsque x=0, on trouve y=-c/b=−r[2]/r[1]'''
            x1, y1 = map(int, [c, -(r[2] + r[0] * c) / r[1]])
            '''x1 est fixé à c, représentant le bord droit de 
            l'image (la largeur de l'image). y1 est calculé de 
            manière similaire pour trouver où la ligne coupe le 
            côté droit de l'image : y=−(r[2]+r[0]⋅c)/r[1]'''
            cv.line(img, (x0, y0), (x1, y1), (0, 255, 0), 1)
            cv.circle(img, tuple(pt), 5, (255, 0, 0), -1)
        return img

    # Sélection d'un point pour afficher les droites épipolaires associées
    def on_click_show_epiline(event, x, y, flags, param):
        if event == cv.EVENT_LBUTTONDOWN:
            if param == 'img1':
                # Calculer les droites épipolaires sur la deuxième image
                lines = cv.computeCorrespondEpilines(np.array([[x, y]]), 1, F)
                #liste de triplet [a,b,c] representant la droite ax+by+c=0
                img2_with_lines = draw_epilines(img2.copy(), lines, pts2)
                cv.imshow('Image 2', img2_with_lines)
            elif param == 'img2':
                # Calculer les droites épipolaires sur la première image
                lines = cv.computeCorrespondEpilines(np.array([[x, y]]), 2, F)
                img1_with_lines = draw_epilines(img1.copy(), lines, pts1)
                cv.imshow('Image 1', img1_with_lines)

    # Après avoir calculé F, configurer les clics pour afficher les droites épipolaires
    print("Affichage des droites")
    cv.imshow('Image 1', img1)
    cv.imshow('Image 2', img2)
    cv.setMouseCallback('Image 1', on_click_show_epiline, param='img1')
    cv.setMouseCallback('Image 2', on_click_show_epiline, param='img2')

    # Attendre pour visualiser les résultats
    cv.waitKey(0)
    cv.destroyAllWindows()
else:
    print("Pas assez de points de correspondance sélectionnés (8 minimum).")
