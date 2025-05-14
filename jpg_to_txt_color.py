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

image = cv2.imread(input_path, cv2.IMREAD_COLOR)

if image is None:
    print(f"Error: Image '{input_path}' not found or cannot be read!")
    exit(1)

height, width = image.shape[:2]
image_name = os.path.splitext(os.path.basename(image_path))[0]
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, f"{image_name}_r.txt")

with open(output_path, 'w') as f:
    f.write(f"{width} {height}\n")
    for row in image:
        row_data = [f"{pixel[2]}" for pixel in row]
        f.write(' '.join(row_data) + '\n')

print(f"Image converted successfully to '{output_path}'!")

height, width = image.shape[:2]
image_name = os.path.splitext(os.path.basename(image_path))[0]
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, f"{image_name}_g.txt")

with open(output_path, 'w') as f:
    f.write(f"{width} {height}\n")
    for row in image:
        row_data = [f"{pixel[1]}" for pixel in row]
        f.write(' '.join(row_data) + '\n')

print(f"Image converted successfully to '{output_path}'!")

height, width = image.shape[:2]
image_name = os.path.splitext(os.path.basename(image_path))[0]
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, f"{image_name}_b.txt")

with open(output_path, 'w') as f:
    f.write(f"{width} {height}\n")
    for row in image:
        row_data = [f"{pixel[0]}" for pixel in row]
        f.write(' '.join(row_data) + '\n')

print(f"Image converted successfully to '{output_path}'!")
