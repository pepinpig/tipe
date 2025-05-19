import cv2 as cv
import matplotlib.pyplot as plt

# Charger les images en couleur (pas en niveaux de gris)
img1 = cv.imread('points/images/dodecf4.jpg')  # BGR
img2 = cv.imread('points/images/dodecf5.jpg')  # BGR

# Vérification du chargement
if img1 is None or img2 is None:
    print("Erreur de chargement des images.")
    exit()

# Détection SIFT
sift = cv.SIFT_create()
kp1, des1 = sift.detectAndCompute(img1, None)
kp2, des2 = sift.detectAndCompute(img2, None)

# Brute-Force Matching + ratio test
bf = cv.BFMatcher()
matches = bf.knnMatch(des1, des2, k=2)

# Ratio de Lowe
good_matches = [m for m, n in matches if m.distance < 0.75 * n.distance]

# Dessiner les correspondances (images couleur)
matched_img = cv.drawMatches(
    img1, kp1, img2, kp2, good_matches, None,
    flags=cv.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS
)

# Convertir BGR vers RGB pour matplotlib
matched_img_rgb = cv.cvtColor(matched_img, cv.COLOR_BGR2RGB)

# Affichage avec matplotlib
plt.figure(figsize=(12, 6))
plt.imshow(matched_img_rgb)
plt.title("Appariement SIFT ")
plt.axis('off')
plt.tight_layout()
plt.show()
