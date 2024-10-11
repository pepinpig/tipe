import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from matplotlib.ticker import LinearLocator

color=cm.coolwarm

# Coordonnées
XO=0
YO=0
XG = 100 
YG = 100
obstacle_list= [[65,80,3, 500, 300],[30,15,8, 1000, 500],[15,35,3, 500, 300]]#Xobs, Yobs, obs_size, η, R0

def Z_calcule(Xobs, Yobs, obs_size, η, R0):
	Robs = np.sqrt((Xobs - X)**2 + (Yobs - Y)**2)
	# Calculer les valeurs de Z obstacle avec une fonction vectorisée
	Zobs = np.where(Robs > R0, 0, η * (((1 / Robs) - (1 / R0))**2) / 2)
	# Définir l'obstacle de taille 1x1 centré en (Xobs, Yobs)
	obstacle_mask = (np.abs(X - Xobs) <= ((obs_size)/2)) & (np.abs(Y - Yobs) <= ((obs_size)/2))
	Zobs[obstacle_mask] = 8
	return Zobs

def trace_obstacle(Xobs, Yobs, obs_size,η, R0):
	# Ajouter un rectangle pour représenter l'obstacle
	obstacle_rect = plt.Rectangle((Xobs - ((obs_size)/2), Yobs - ((obs_size)/2)), obs_size, obs_size, linewidth=1, edgecolor='r', facecolor='r')
	ax2.add_patch(obstacle_rect)
	ax2.annotate('Obstacle', xy=(Xobs, Yobs), xytext=(Xobs,Yobs))


# Créer la figure avec deux sous-graphiques
fig = plt.figure(figsize=(12, 6))

# Premier sous-graphe : surface en 3D
ax1 = fig.add_subplot(121, projection='3d')

# Générer les données
X = np.arange(0, 100, 0.25)
Y = np.arange(0, 100, 0.25)
X, Y = np.meshgrid(X, Y)

# Z goal 
k=0.0003
RG = np.sqrt((X - XG)**2 + (Y - YG)**2)
ZG = k*(RG**2) / 2

# Combiner Z goal et Z obstacle
Z =ZG
for obs in obstacle_list:
	Z += Z_calcule(obs[0], obs[1], obs[2], obs[3], obs[4])
# Troncature des valeurs de Zob à 10
Z = np.minimum(Z, 10)

# Tracer la surface
surf = ax1.plot_surface(X, Y, Z, cmap=color, linewidth=0, antialiased=False)

# Personnaliser l'axe Z
ax1.set_zlim(-0.5, 10.5)
ax1.zaxis.set_major_locator(LinearLocator(10))
ax1.zaxis.set_major_formatter('{x:.01f}')

# Ajouter une barre de couleur qui associe les valeurs aux couleurs
fig.colorbar(surf, ax=ax1, shrink=0.5, aspect=10)

# Ajouter les lignes de niveau projetées sur le plan x, y
ax1.contour(X, Y, Z, zdir='z', offset=-0.5, cmap=color)

# Deuxième sous-graphe : graphique des lignes de niveau sur le plan x, y
ax2 = fig.add_subplot(122)

# Tracer les lignes de niveau
contour = ax2.contour(X, Y, Z, cmap=color)
ax2.clabel(contour, inline=True, fontsize=8)
ax2.set_title('Lignes de niveau sur le plan x-y')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')

# Ajouter un point pour l'objectif
ax2.plot(XG, YG, 'ko')  # Point noir pour l'objectif
ax2.annotate('Objectif', xy=(XG, YG), xytext=(XG + 0.5, YG + 0.5))
for obs in obstacle_list:
	trace_obstacle(obs[0], obs[1], obs[2], obs[3], obs[4])

 
# Afficher les graphiques
plt.show()
