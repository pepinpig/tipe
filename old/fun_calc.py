import matplotlib.pyplot as plt
from sympy import symbols, sqrt, Piecewise, lambdify
import numpy as np

# Define the sympy symbols to be used in the function
x, y = symbols('x y')

def create_obstacle_function(Xobs, Yobs, obs_size, η, R0):
    Robs = sqrt((Xobs - x)**2 + (Yobs - y)**2)
    # Calculer les valeurs de Z obstacle avec une fonction symbolique
    Zobs = η * (((1 / Robs) - (1 / R0))**2) / 2
    Zobs = Piecewise((Zobs, Robs <= R0), (0, True))
    
    # Print the function in the form f(x, y) = z
    print(f"f(x, y) = {Zobs}")
    
    # Créer une fonction lambda pour la partie symbolique
    obstacle_func = lambdify((x, y), Zobs, modules='numpy')
    
    # Définir une fonction numérique qui applique le masque d'obstacle
    def f(X, Y):
        Zobs_numeric = obstacle_func(X, Y)
        obstacle_mask = (np.abs(X - Xobs) <= ((obs_size) / 2)) & (np.abs(Y - Yobs) <= ((obs_size) / 2))
        Zobs_numeric[obstacle_mask] = 8
        return Zobs_numeric
    
    return f

# Utilisation de la fonction pour créer un obstacle
obstacle_function = create_obstacle_function(65, 80, 3, 500, 300)

# Générer les données pour X et Y
X = np.linspace(-100, 100, 200)
Y = np.linspace(-100, 100, 200)
X, Y = np.meshgrid(X, Y)

# Calculer les valeurs de Z pour l'obstacle
Z = obstacle_function(X, Y)

# Afficher les résultats pour vérifier
print(Z)

# Optionnel : visualisation
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, cmap='viridis')
plt.show()
