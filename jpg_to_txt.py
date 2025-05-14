import cv2
import numpy as np
import sys
import os

if len(sys.argv) != 2:
    print("Usage: python script.py <image>.jpg")
    exit(1)

image_path = sys.argv[1]
input_dir = "points/images/"
output_dir = "points/donnees/"
input_path = os.path.join(input_dir, image_path)


image = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)


if image is None:
    print(f"Error: Image '{input_path}' not found or cannot be read!")
    exit(1)

height, width = image.shape

image_name = os.path.splitext(os.path.basename(image_path))[0]

os.makedirs(output_dir, exist_ok=True) 
output_path = os.path.join(output_dir, f"{image_name}.txt")

with open(output_path, 'w') as f:
    f.write(f"{width} {height}\n")  # Écrire les dimensions sur la première ligne
    for row in image:
        row_str = ' '.join(map(str, row))  # Convertir chaque pixel en string
        f.write(row_str + '\n')  # Écrire la ligne dans le fichier

print(f"Image converted successfully to '{output_path}'!")
