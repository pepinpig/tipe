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
output_dir = "points/images/"
input_path = os.path.join(output_dir, image_path)
# Vérifier que l'image a bien l'extension .jpg
if not image_path.lower().endswith(".jpg"):
    print("Error: The input file must be a .jpg image!")
    exit(1)

# Charger l'image en niveaux de gris
image = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)

# Vérifier si l'image est chargée correctement
if image is None:
    print(f"Error: Image '{image_path}' not found!")
    exit(1)

# Récupérer le nom de base sans extension
image_name = os.path.splitext(os.path.basename(image_path))[0]

# Définir le chemin du fichier de sortie
os.makedirs(output_dir, exist_ok=True)  # Créer le dossier s'il n'existe pas
output_path = os.path.join(output_dir, f"{image_name}.txt")

# Ouvrir le fichier en écriture et stocker les pixels
with open(output_path, 'w') as f:
    for row in image:
        row_str = ' '.join(map(str, row))  # Convertir chaque pixel en string
        f.write(row_str + '\n')  # Écrire la ligne dans le fichier

print(f"Image converted successfully to '{output_path}'!")
