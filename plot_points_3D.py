import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import sys

def lire_et_tracer_points(fichiers):
    try:
        # Initialiser la figure 3D
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')

        # Couleurs pour différencier les fichiers
        couleurs = ['b', 'g', 'r', 'c', 'm', 'y', 'k']

        # Initialiser les bornes pour garder un repère orthonormé
        x_min, x_max = float('inf'), float('-inf')
        y_min, y_max = float('inf'), float('-inf')
        z_min, z_max = float('inf'), float('-inf')

        for index, fichier in enumerate(fichiers):
            x_coords = []
            y_coords = []
            z_coords = []
            complete_name = f'points/donnees/{fichier}.txt'
            with open(complete_name, 'r') as f:
                for ligne in f:
                    x, y, z = map(float, ligne.strip().split())
                    x_coords.append(x)
                    y_coords.append(y)
                    z_coords.append(z)

            # Mettre à jour les bornes
            x_min, x_max = min(x_min, min(x_coords)), max(x_max, max(x_coords))
            y_min, y_max = min(y_min, min(y_coords)), max(y_max, max(y_coords))
            z_min, z_max = min(z_min, min(z_coords)), max(z_max, max(z_coords))

            # Choisir une couleur pour ce fichier
            couleur = couleurs[index % len(couleurs)]
            ax.scatter(x_coords, y_coords, z_coords, c=couleur, marker='o', label=f"Fichier {fichier}")

            # Ajouter des étiquettes pour chaque point
            for i, (x, y, z) in enumerate(zip(x_coords, y_coords, z_coords)):
                ax.text(x, y, z, f"{i + 1}", color=couleur)

        # Définir les mêmes échelles pour chaque axe
        max_range = max(x_max - x_min, y_max - y_min, z_max - z_min) / 2.0
        mid_x = (x_max + x_min) / 2.0
        mid_y = (y_max + y_min) / 2.0
        mid_z = (z_max + z_min) / 2.0
        ax.set_xlim(mid_x - max_range, mid_x + max_range)
        ax.set_ylim(mid_y - max_range, mid_y + max_range)
        ax.set_zlim(mid_z - max_range, mid_z + max_range)

        # Ajouter des étiquettes des axes
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.title('Points 3D')
        plt.legend()
        plt.show()

    except FileNotFoundError as e:
        print(f"Erreur : Le fichier est introuvable : {e}")
    except ValueError as e:
        print(f"Erreur de format dans un des fichiers : {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage : plot_points.py <nom_du_fichier1> <nom_du_fichier2> ...")
    else:
        fichiers = sys.argv[1:]
        lire_et_tracer_points(fichiers)
