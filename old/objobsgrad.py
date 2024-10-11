import matplotlib.pyplot as plt
import numpy as np
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import sympy as sp
from sympy import symbols, diff, lambdify, sqrt, Piecewise, srepr

color = cm.coolwarm

x_min, x_max = 0, 100
y_min, y_max = 0, 100

# Define the sympy symbols to be used in the function
x, y = symbols('x y')

# Select a start point
x0, y0 = 3, 3
XG = 90
YG = 90
obstacle_list = [[60, 80, 10, 1500, 500], [10, 60, 5, 800, 300], [80, 50, 3, 500, 100]]  # Xobs, Yobs, obs_size, η, R0
#[60, 60, 10, 1500, 500], [10, 60, 5, 800, 300], [80, 20, 3, 500, 100]
def Z_calcule_obstacle(Xobs, Yobs, obs_size, η, R0):
    Robs = sqrt((Xobs - x)**2 + (Yobs - y)**2)
    # Calculer les valeurs de Z obstacle avec une fonction symbolique
    Zobs = η * (((1 / Robs) - (1 / R0))**2) / 2
    Zobs = Piecewise((Zobs, Robs <= R0), (0, True))
    return Zobs

def Z_calcule_border(η,R0):
        # Ensure x and y are within the boundaries
    x = symbols('x', real=True, positive=True, bounded=(0, 110))
    y = symbols('y', real=True, positive=True, bounded=(0, 110))
    # Calculer les valeurs de Z obstacle avec une fonction symbolique
    def Zbord(z,z_m) :
        return η * (((1 / sqrt((z-z_m)**2)) - (1 / R0))**2) / 2
    Zbord = Piecewise(
        (Zbord(x,x_min),(x-x_min) <= R0), 
        (Zbord(y,y_min),(y-y_min) <= R0),
        (Zbord(x,x_max),(x_max-x) <= R0),
        (Zbord(y,y_max),(y_max-y) <= R0),
        (0, True))
    return Zbord



def trace_obstacle(ax, Xobs, Yobs, obs_size, η, R0):
    # Ajouter un rectangle pour représenter l'obstacle
    obstacle_rect = plt.Rectangle((Xobs - (obs_size / 2), Yobs - (obs_size / 2)), obs_size, obs_size, linewidth=1, edgecolor='r', facecolor='r')
    ax.add_patch(obstacle_rect)
    ax.annotate('Obstacle', xy=(Xobs, Yobs), xytext=(Xobs, Yobs))



# Créer la figure avec deux sous-graphiques
fig = plt.figure(figsize=(12, 6))

# Premier sous-graphe : surface en 3D
ax1 = fig.add_subplot(121, projection='3d')

# Générer les données
X = np.arange(0, 100, 0.25)
Y = np.arange(0, 100, 0.25)
X, Y = np.meshgrid(X, Y)

# Z goal
k = 0.001
RG = sqrt((x - XG)**2 + (y - YG)**2)
ZG = k * (RG**2) / 2

# Combine Z goal and Z obstacle
f1 = ZG
for obs in obstacle_list:
    f1 += Z_calcule_obstacle(obs[0], obs[1], obs[2], obs[3], obs[4])
print(sp.srepr(f1))
# Define a function optimized for numpy array calculation
f = lambdify([x, y], f1, 'numpy')
# Troncature des valeurs de Zob à 10
Z = f(X, Y)
Z = np.minimum(Z, 10)

# Calculate the partial derivatives of f1 w.r.t. x and y
f1x = diff(f1, x)
f1y = diff(f1, y)

# Define the x and y grid arrays
x_grid = np.arange(x_min, x_max, 0.25)
y_grid = np.arange(y_min, y_max, 0.25)
# Create mesh grid for surface plot
X, Y = np.meshgrid(x_grid, y_grid)


# Tracer la surface
surf = ax1.plot_surface(X, Y, Z, cmap=color, linewidth=0, antialiased=False,alpha=0.8)

# Personnaliser l'axe Z
ax1.set_zlim(-0.5, 10.5)
ax1.zaxis.set_major_locator(LinearLocator(10))
ax1.zaxis.set_major_formatter('{x:.01f}')

# Ajouter une barre de couleur qui associe les valeurs aux couleurs
fig.colorbar(surf, ax=ax1, shrink=0.5, aspect=10)

# Ajouter les lignes de niveau projetées sur le plan x, y
ax1.view_init(elev=30, azim=20)  # Adjust elevation and azimuth angles as needed
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
    trace_obstacle(ax2, obs[0], obs[1], obs[2], obs[3], obs[4])

# Gradient descent
# Convert symbolic gradient to numerical functions
grad_x = lambdify([x, y], f1x, 'numpy')
grad_y = lambdify([x, y], f1y, 'numpy')


# Initialize a list for storing the gradient descent points
xlist = [x0]
ylist = [y0]

# Specify the learning rate
lr = 100

# Perform gradient descent
for i in range(100):
    # Update the x and y values using the negative gradient values
    x0 -= grad_x(x0, y0) * lr
    y0 -= grad_y(x0, y0) * lr
    # Append to the list to keep track of the points
    xlist.append(x0)
    print(x0,y0)
    ylist.append(y0)

xarr = np.array(xlist, dtype='float64')
yarr = np.array(ylist, dtype='float64')
zlist = list(f(xarr, yarr))
# Plot the gradient descent path
ax1.scatter(xlist, ylist,zlist, color='red', s=20, marker='o')
ax2.plot(xlist, ylist, 'ro-', markersize=5, alpha=0.6)
ax2.set_title('Descente de gradient')

plt.show()