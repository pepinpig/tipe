import cv2 as cv
import numpy as np

# Charger les images
img1 = cv.imread('IMG_0078.jpg')
img2 = cv.imread('IMG_0079.jpg')


# Redimensionner les images pour qu'elles aient la même hauteur
h1, w1, _ = img1.shape
h2, w2, _ = img2.shape
if h1 != h2:
    new_width1 = int(w1 * (h2 / h1))
    img1 = cv.resize(img1, (new_width1, h2))

# Concaténer les images horizontalement
combined_image = np.hstack((img1, img2))

# Lors d'un clique gauche sur l'image concaténée, affiche la position 
#(x,y) du point cliqué et l'image à lquelle il appartient

def mouse_click(event, x, y, flags, param):
    if event == cv.EVENT_LBUTTONDOWN:
        if x < img1.shape[1]:
            # Le clic est dans la première image
            print(f"Position dans image 1 : x = {x}, y = {y}")
        else:
            # Le clic est dans la deuxième image
            relative_x = x - img1.shape[1]  # Ajuster par rapport à l'image 2
            print(f"Position dans image 2 : x = {relative_x}, y = {y}")


# Afficher l'image concaténée 
cv.imshow('Images', combined_image)
cv.setMouseCallback('Images', mouse_click)

# Attendre que l'utilisateur ferme la fenêtre
cv.waitKey(0)
cv.destroyAllWindows()
