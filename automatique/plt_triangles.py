import matplotlib.pyplot as plt
import numpy as np
import sys

def read_triangles(file_path):
    """Lit un fichier contenant des triangles"""
    triangles = []
    with open(file_path, "r") as f:
        triangle = []
        for line in f:
            if line.strip() == "---":
                if triangle:
                    triangles.append(np.array(triangle))
                    triangle = []
            else:
                triangle.append(list(map(float, line.strip().split())))
        if triangle:
            triangles.append(np.array(triangle))
    return triangles

def plot_triangles(triangles):
    """Affiche les triangles avec Matplotlib"""
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    for triangle in triangles:
        # Boucler sur chaque triangle et l'afficher
        x, y, z = triangle[:, 0], triangle[:, 1], triangle[:, 2]
        ax.plot_trisurf(x, y, z, alpha=0.5, edgecolor='k')  # Surface du triangle
        ax.scatter(x, y, z, color='r')  # Sommets du triangle
    
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")
    plt.show()

if __name__ == "__main__":
    fichier = sys.argv[1]
    complete_fichier = f'points/donnees/tri_points_3D-{fichier}0.txt'
    triangles = read_triangles(complete_fichier)
    plot_triangles(triangles)
