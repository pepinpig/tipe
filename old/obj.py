import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from matplotlib.ticker import LinearLocator

# Créer la figure avec deux sous-graphiques
fig = plt.figure(figsize=(12, 6))

# Premier sous-graphe : surface en 3D
ax1 = fig.add_subplot(121, projection='3d')

# Générer les données
X = np.arange(-2, 2, 0.25)
Y = np.arange(-2, 2, 0.25)
X, Y = np.meshgrid(X, Y)
R = np.sqrt(X**2 + Y**2)
Z = (R**2) / 2

# Tracer la surface
surf = ax1.plot_surface(X, Y, Z, cmap=cm.coolwarm, linewidth=0, antialiased=False)

# Personnaliser l'axe Z
ax1.set_zlim(-0.5, 2.0)
ax1.zaxis.set_major_locator(LinearLocator(10))
ax1.zaxis.set_major_formatter('{x:.01f}')

# Ajouter une barre de couleur qui associe les valeurs aux couleurs
fig.colorbar(surf, ax=ax1, shrink=0.5, aspect=10)

# Ajouter les lignes de niveau projetées sur le plan x, y
ax1.contour(X, Y, Z, zdir='z', offset=-0.5, cmap=cm.coolwarm)

# Deuxième sous-graphe : graphique des lignes de niveau sur le plan x, y
ax2 = fig.add_subplot(122)

# Tracer les lignes de niveau
contour = ax2.contour(X, Y, Z, cmap=cm.coolwarm)
ax2.clabel(contour, inline=True, fontsize=8)
ax2.set_title('Lignes de niveau sur le plan x-y')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')

# Afficher les graphiques
plt.show()
