import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import numpy as np

# Création de la figure et d'un axe 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Fonction pour tracer un point
def point(x, y, z, nom, couleur):
    ax.scatter([x], [y], [z], color=couleur, s=10)
    ax.text(x, y, z, nom, color='black') 

# Coordonnées des 8 sommets du pavé
vertices = list(zip(
    [4+x for x in [0, 1, 1, 0, 0, 1, 1, 0]],  # x
    [0, 0, 1, 1, 0, 0, 1, 1],  # y
    [0, 0, 0, 0, 1, 1, 1, 1]   # z
))

# Liste des faces
faces = [
    [vertices[0], vertices[1], vertices[2], vertices[3]],  # Face avant
    [vertices[4], vertices[5], vertices[6], vertices[7]],  # Face arrière
    [vertices[0], vertices[3], vertices[7], vertices[4]],  # Face gauche
    [vertices[1], vertices[2], vertices[6], vertices[5]],  # Face droite
    [vertices[3], vertices[2], vertices[6], vertices[7]],  # Face supérieure
    [vertices[0], vertices[1], vertices[5], vertices[4]]   # Face inférieure
]

# Ajouter le pavé à la figure
ax.add_collection3d(Poly3DCollection(faces, facecolors='yellow', linewidths=1, edgecolors='orange', alpha=.25))

# Coordonnées du point P
x_p, y_p, z_p = 4, 0.8, 1
point(x_p, y_p, z_p, 'P', 'red')  # Point P sur le pavé

# Coordonnées du point C
x_c, y_c, z_c = 0, 0.5, 0.5
point(x_c, y_c, z_c, 'C', 'black')

# Tracer le plan correspondant au rectangle
x_square = [1, 1, 1, 1]
y_square = [0, 1, 1, 0]
z_square = [0, 0, 1, 1]
verts = [list(zip(x_square, y_square, z_square))]
ax.add_collection3d(Poly3DCollection(verts, facecolors='grey', linewidths=1, edgecolors='black', alpha=.25))

# Coordonnées du point O
x_O, y_O, z_O = 1, 0.5, 0.5
point(x_O, y_O, z_O, 'O', 'black')

# Tracer les segments CP
ax.plot([x_p, x_c], [y_p, y_c], [z_p, z_c], 'red')

# Trouver l'intersection entre la droite CP et le plan x = 1
# L'équation de la droite : r(t) = C + t(P - C)
# Soit : x(t) = x_c + t(x_p - x_c)
# Nous voulons x(t) = 1

t_intersection = (1 - x_c) / (x_p - x_c)

# Calculer les coordonnées de l'intersection
x_intersection = x_c + t_intersection * (x_p - x_c)
y_intersection = y_c + t_intersection * (y_p - y_c)
z_intersection = z_c + t_intersection * (z_p - z_c)

# Tracer le point d'intersection P'
point(x_intersection, y_intersection, z_intersection, "P'", 'blue')

# Tracer le point O'
x_os,y_os,z_os=4,0.5,0.5
point(x_os,y_os,z_os,"O'",'grey')   # Point P sur le pavé

ax.plot([x_os,x_c],[y_os,y_c], [z_os,z_c],'grey')


#parmamètres affichage
x_scale=5/2
y_scale=2/2
z_scale=2/2
ax.set_xlim([0, 5])
ax.set_ylim([-0.5,1.5])
ax.set_zlim([0, 2])

# Tracer les axes X, Y, Z avec des flèches
point(0,0,0,"R",'black')
ax.quiver(0, 0, 0, 1*x_scale, 0, 0, color='black', arrow_length_ratio=0.1)
ax.text(1*x_scale, 0, 0, 'X', color='black') 
ax.quiver(0, 0, 0, 0, 1*y_scale, 0, color='black', arrow_length_ratio=0.1)
ax.text(0, 1*y_scale, 0, 'Y', color='black') 
ax.quiver(0, 0, 0, 0, 0, 1*z_scale, color='black', arrow_length_ratio=0.1)
ax.text(0, 0, 1*z_scale, 'Z', color='black') 

# Tracer les axes x_c, y_c, z_c avec des flèches
ax.quiver(x_c, y_c, z_c, 0.3*x_scale, 0, 0, color='black', arrow_length_ratio=0.1)
ax.text(x_c+0.3*x_scale, y_c, z_c, '$x_c$', color='black') 
ax.quiver(x_c, y_c, z_c,0, 0.3*y_scale,0, color='black', arrow_length_ratio=0.1)
ax.text(x_c, y_c+0.3*y_scale, z_c, '$y_c$', color='black') 
ax.quiver(x_c, y_c, z_c, 0,0,0.3*z_scale, color='black', arrow_length_ratio=0.1)
ax.text(x_c, y_c, z_c+0.3*z_scale, '$z_c$', color='black') 

# Tracer les axes u, v avec des flèches
ax.quiver(1, 0, 0, 0, 0,0.3*z_scale,color='blue', arrow_length_ratio=0.1)
ax.text(1, 0, 0+0.3*z_scale, '$v$', color='blue') 
ax.quiver(1, 0, 0, 0, 0.3*y_scale, 0, color='blue', arrow_length_ratio=0.1)
ax.text(1, 0+0.3*y_scale, 0, '$u$', color='blue')  


# ax.set_xlabel('X')
# ax.set_ylabel('Y')
# ax.set_zlabel('Z')

# ax.set_xticks(np.arange(0, 5.5, 1))
# ax.set_yticks(np.arange(0, 2.5, 1))
# ax.set_zticks(np.arange(0, 2.5, 1))
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])
ax.set_axis_off()

plt.show()
