import matplotlib.pyplot as plt
import numpy as np
import sys


def load_points(filename):
    """
    Charge les coordonnées des points depuis un fichier texte.
    Le fichier doit contenir les coordonnées sous la forme : x y
    """
    points = np.loadtxt(filename)
    return points


def plot_images_with_points(image1, image2, points1, points2):
    """
    Affiche les deux images avec les points respectivement à leurs coordonnées données.
    """
    fig, ax = plt.subplots(1, 2, figsize=(12, 6))

    # Génération des couleurs aléatoires pour chaque point ou chaque correspondance
    colors = np.random.rand(len(points1), 3)

    # Affichage de l'image 1
    ax[0].imshow(image1, cmap='gray')
    
    # Affichage de l'image 2
    ax[1].imshow(image2, cmap='gray')
    
    for i in range(min(len(points1), len(points2))):
        color = np.random.rand(3,)
        ax[0].scatter(points1[i, 0], points1[i, 1], color=color, label='Points Image 1' if i == 0 else "")
        ax[1].scatter(points2[i, 0], points2[i, 1], color=color, label='Points Image 2' if i == 0 else "")
        
        
    ax[0].set_title('Image 1 avec Points')
    ax[1].set_title('Image 2 avec Points')
    ax[0].legend()
    ax[1].legend()
    plt.show()


def main(image1_name, image2_name, points1_name, points2_name):
    """
    Fonction principale pour charger les images et les points, puis les afficher.
    """
    # Définition des chemins des fichiers
    image1_path = f"points/images/{image1_name}"
    image2_path = f"points/images/{image2_name}"
    points1_path = f"points/donnees/{points1_name}"
    points2_path = f"points/donnees/{points2_name}"

    # Chargement des images
    image1 = plt.imread(image1_path)
    image2 = plt.imread(image2_path)

    # Chargement des coordonnées des points
    points1 = load_points(points1_path)
    points2 = load_points(points2_path)

    # Affichage des images avec les points
    plot_images_with_points(image1, image2, points1, points2)


if __name__ == '__main__':
    if len(sys.argv) != 5:
        print("Usage: python script.py <image1_name> <image2_name> <points1_name> <points2_name> ")
        sys.exit(1)
    
    image1_name = sys.argv[1]
    image2_name = sys.argv[2]
    points1_name = sys.argv[3]
    points2_name = sys.argv[4]
    
    main(image1_name, image2_name, points1_name, points2_name)
