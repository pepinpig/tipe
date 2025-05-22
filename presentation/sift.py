
import os
os.environ["QT_QPA_PLATFORM"] = "offscreen"  # <-- Ajoutez cette ligne
import matplotlib
matplotlib.use('Agg')  # backend non interactif, génère des images sans affichage
import matplotlib.pyplot as plt
import cv2 as cv
# Charger les images en couleur (pas en niveaux de gris)
img1 = cv.imread('../points/images/dodecf0.jpg')  # BGR
img2 = cv.imread('../points/images/dodecf1.jpg')  # BGR

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
plt.axis('off')
plt.tight_layout()
# Au lieu de plt.show()
plt.savefig("appariement_sift.png")
print("Image enregistrée dans : appariement_sift.png")
