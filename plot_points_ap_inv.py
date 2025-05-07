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
    Affiche les deux images concaténées avec les points respectivement à leurs coordonnées données.
    Affiche aussi les numéros des points et trace une ligne entre chaque i-ème point des deux images.
    """
    import numpy as np
    import matplotlib.pyplot as plt

    # Concaténation des images horizontalement
    combined_image = np.hstack((image1, image2))
    
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.imshow(combined_image, cmap='gray')
    
    # Génération des couleurs aléatoires pour chaque correspondance
    colors = np.random.rand(len(points1), 3)
    
    for i in range(min(len(points1), len(points2))):
        color = colors[i]
        # Point image 1
        ax.scatter(points1[i, 1], points1[i, 0], color=color, label='Points Image 1' if i == 0 else "")
        ax.text(points1[i, 1] + 3, points1[i, 0] - 3, str(i), color=color, fontsize=11)

        # Point image 2
        if (points2[i, 0] + points2[i, 1] > 0):
            ax.scatter(points2[i, 1] + image1.shape[1], points2[i, 0], color=color, label='Points Image 2' if i == 0 else "")
            ax.text(points2[i, 1] + image1.shape[1] + 3, points2[i, 0] - 3, str(i), color=color, fontsize=11)

            # Ligne entre les points
            ax.plot([points1[i, 1], points2[i, 1] + image1.shape[1]],
                    [points1[i, 0], points2[i, 0]], color=color, linestyle='-', linewidth=1)
    
    ax.set_title('Points appariés avec numéros')
    ax.legend()
    plt.axis('off')
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
        print("Usage: python script.py <image1_name> <image2_name> <points1_name> <points2_name>")
        sys.exit(1)
    
    image1_name = sys.argv[1]
    image2_name = sys.argv[2]
    points1_name = sys.argv[3]
    points2_name = sys.argv[4]
    
    main(image1_name, image2_name, points1_name, points2_name)