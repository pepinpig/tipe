import cv2
import numpy as np
import sys
import os

# Vérifier que l'argument est fourni
if len(sys.argv) != 2:
    print("Usage: python script.py <image>.jpg")
    exit(1)

# Récupérer le nom de l'image depuis les arguments
image_path = sys.argv[1]
input_path = "points/donnees/"+image_path+"-out.pbm"

image = cv2.imread(input_path)

# Vérifier si l'image a été correctement chargée
if image is None:
    print("Erreur lors du chargement de l'image.")
else:
    # Afficher l'image dans une fenêtre
    cv2.imshow("Image pbm", image)

    # Attendre que l'utilisateur appuie sur une touche pour fermer la fenêtre
    cv2.waitKey(0)
    cv2.destroyAllWindows()
