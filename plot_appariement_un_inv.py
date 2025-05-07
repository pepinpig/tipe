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


def plot_image_with_points(image, points):
    """
    Affiche une image avec des points superposés.
    """
    fig, ax = plt.subplots(figsize=(8, 6))

    # Génération des couleurs aléatoires pour chaque point
    colors = np.random.rand(len(points), 3)

    ax.imshow(image, cmap='gray')

    for i in range(len(points)):
        ax.scatter(points[i, 0], points[i, 1], color=colors[i], label='Points' if i == 0 else "")

    plt.show()


def main(image_name, points_name):
    """
    Fonction principale pour charger l'image et les points, puis les afficher.
    """
    # Définition des chemins des fichiers
    image_path = f"points/images/{image_name}"
    points_path = f"points/donnees/{points_name}"

    # Chargement de l'image
    image = plt.imread(image_path)

    # Chargement des coordonnées des points
    points = load_points(points_path)

    # Affichage de l'image avec les points
    plot_image_with_points(image, points)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python script.py <image_name> <points_name>")
        sys.exit(1)
    
    image_name = sys.argv[1]
    points_name = sys.argv[2]
    
    main(image_name, points_name)
