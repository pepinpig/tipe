import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def draw_cube(ax, origin=[0, 0, 0], size=1):
    o = np.array(origin)
    r = size
    vertices = [
        o,
        o + [r, 0, 0],
        o + [r, r, 0],
        o + [0, r, 0],
        o + [0, 0, r],
        o + [r, 0, r],
        o + [r, r, r],
        o + [0, r, r]
    ]
    vertices = np.array(vertices)

    edges = [
        [0, 1], [1, 2], [2, 3], [3, 0],
        [4, 5], [5, 6], [6, 7], [7, 4],
        [0, 4], [1, 5], [2, 6], [3, 7]
    ]
    for edge in edges:
        ax.plot3D(*zip(*vertices[edge]), color="black")

def draw_axes(ax, origin=[0, 0, 0], length=1):
    o = np.array(origin)
    ax.quiver(*o, 1, 0, 0, color="red", length=length, normalize=True)
    ax.quiver(*o, 0, 1, 0, color="green", length=length, normalize=True)
    ax.quiver(*o, 0, 0, 1, color="blue", length=length, normalize=True)
    ax.text(*o + [1.1*length, 0, 0], "X", color="red")
    ax.text(*o + [0, 1.1*length, 0], "Y", color="green")
    ax.text(*o + [0, 0, 1.1*length], "Z", color="blue")

# Figure sans grille ni ticks
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_box_aspect([1, 1, 1])

# Suppression des ticks et de la grille
ax.grid(False)
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

# Suppression du fond gris (facultatif)
ax.set_facecolor('white')

# Affichage cube + repère
draw_cube(ax, origin=[0, 0, 0], size=1)
draw_axes(ax, origin=[0, 0, 0], length=0.5)

# Définir les limites et la vue
ax.set_xlim([0, 1.5])
ax.set_ylim([0, 1.5])
ax.set_zlim([0, 1.5])
ax.view_init(elev=20, azim=30)

plt.tight_layout()
plt.show()
