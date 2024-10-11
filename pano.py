import cv2
import numpy as np
import matplotlib.pyplot as plt

# Fonction pour afficher les images côte à côte
def show_images_side_by_side(image1, image2, title1=None, title2=None):
    fig, axs = plt.subplots(1, 2, figsize=(12, 6))
    
    axs[0].imshow(image1)
    if title1:
        axs[0].set_title(title1)
    axs[0].axis('off')

    axs[1].imshow(image2)
    if title2:
        axs[1].set_title(title2)
    axs[1].axis('off')

    plt.show()

# Fonction pour effectuer la projection sphérique d'une image
def warp_spherical(image, K):
    height, width = image.shape[:2]
    sph_image = np.zeros_like(image)

    for y in range(height):
        for x in range(width):
            x_norm = (x - width / 2) / K[0, 0]
            y_norm = (y - height / 2) / K[1, 1]

            theta = np.arctan2(x_norm, 1)
            phi = np.arctan2(y_norm, np.sqrt(x_norm**2 + 1))

            xsph = int(K[0, 0] * theta + width / 2)
            ysph = int(K[1, 1] * phi + height / 2)

            if 0 <= xsph < width and 0 <= ysph < height:
                sph_image[ysph, xsph] = image[y, x]

    return sph_image

# Fonction pour effectuer le blending des images
def blend_images(panorama, images, transformations, feather_width=20):
    height, width = panorama.shape[:2]
    blend_result = np.zeros_like(panorama, dtype=np.float32)
    blend_weight = np.zeros_like(panorama, dtype=np.float32)

    for img, R in zip(images, transformations):
        K = np.array([[800, 0, width / 2], [0, 800, height / 2], [0, 0, 1]])
        warped_img = warp_spherical(img, K)

        # Créer des grilles pour les coordonnées
        y_coords, x_coords = np.indices((height, width))

        # Inverser le mapping pour tous les pixels en même temps
        src_coords = np.linalg.inv(R) @ np.vstack((x_coords.ravel(), y_coords.ravel(), np.ones((1, height * width))))
        src_x = (src_coords[0] * K[0, 0] + K[0, 2]).reshape(height, width).astype(np.int32)
        src_y = (src_coords[1] * K[1, 1] + K[1, 2]).reshape(height, width).astype(np.int32)

        # Masque pour les coordonnées valides
        valid_mask = (src_x >= 0) & (src_x < img.shape[1]) & (src_y >= 0) & (src_y < img.shape[0])

        # Vérifier si le masque valide a des pixels à traiter
        if np.any(valid_mask):
            # Calculer les poids pour le feathering
            weight = calculate_feathering(x_coords, y_coords, feather_width)

            # Appliquer les poids seulement aux pixels valides
            blend_result[y_coords[valid_mask], x_coords[valid_mask]] += warped_img[src_y[valid_mask], src_x[valid_mask]] * weight[valid_mask]
            blend_weight[y_coords[valid_mask], x_coords[valid_mask]] += weight[valid_mask]

    # Normaliser le résultat final
    blend_result /= np.maximum(blend_weight, 1e-10)
    return np.uint8(blend_result)


# Fonction pour inverser le mapping aux coordonnées de l'image source
def inverse_map_to_image(x, y, R, K):
    src_coord = np.linalg.inv(R) @ np.array([[x], [y], [1]])
    
    # Extraction des valeurs pour éviter l'avertissement
    src_x = int(src_coord[0, 0] * K[0, 0] + K[0, 2])
    src_y = int(src_coord[1, 0] * K[1, 1] + K[1, 2])
    
    return src_x, src_y

# Fonction de feathering
def calculate_feathering(x_coords, y_coords, feather_width):
    # Calculer la distance aux bords
    # x_max et y_max doivent être remplacés par la largeur et la hauteur de l'image
    x_max, y_max = x_coords.shape[1], x_coords.shape[0]  # Taille de l'image
    distance_to_edge = np.minimum(np.minimum(x_coords, y_coords), np.minimum(x_max - x_coords, y_max - y_coords))
    
    # Calculer les poids de feathering basés sur la distance aux bords
    weights = np.clip(1 - (distance_to_edge / feather_width), 0, 1)

    return weights
# Fonction principale pour le blending panoramique final
def final_panorama_blending(images, transformations):
    panorama = np.zeros((1000, 3000, 3), dtype=np.uint8)
    blended_panorama = blend_images(panorama, images, transformations)
    return blended_panorama

# Exemple d'utilisation
image1 = cv2.imread('images/telecommande3.jpg')
image2 = cv2.imread('images/telecommande4.jpg')
images = [image1, image2]

# Vérification des images
for i, img in enumerate(images):
    if img is None:
        print(f"Erreur: L'image {i + 1} n'a pas pu être chargée. Vérifie le chemin ou l'intégrité du fichier.")
        exit(1)

# Matrices de transformation (homographies) pour le stitching
R1 = np.eye(3)
R2 = np.array([[0.998, 0.02, 0], [-0.02, 0.998, 0], [0, 0, 1]])

transformations = [R1, R2]

# Appliquer le blending et afficher le panorama final
result = final_panorama_blending(images, transformations)

# Afficher le panorama final
show_images_side_by_side(result, result, title1='Final Panorama', title2='Final Panorama')
