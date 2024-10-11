#import the libraries
from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
from sympy import symbols, diff, lambdify

# Define the sympy symbols to be used in the function
x, y = symbols('x y')

# Define the function in terms of x and y
f1 = (x-2) ** 2 + (y-2) ** 2 + 5

# Calculate the partial derivatives of f1 w.r.t. x and y 
f1x = diff(f1, x)
f1y = diff(f1, y)

# Define a function optimized for numpy array calculation
f = lambdify([x, y], f1, 'numpy')

# Define the x and y grid arrays
x_grid = np.linspace(-3, 3, 30)
y_grid = np.linspace(-3, 3, 30)
# Create mesh grid for surface plot
X, Y = np.meshgrid(x_grid, y_grid)

# Define the surface function using the lambdify function 
Z = f(X, Y)

# Select a start point
x0, y0 = (-3, -3)
# Initialize a list for storing the gradient descent points
xlist = [x0]
ylist = [y0]

# Specify the learning rate
lr = 0.01

# Perform gradient descent
for i in range(100):
    # Update the x and y values using the negative gradient values
    x0 -= f1x.evalf(subs={x: x0, y: y0}) * lr
    y0 -= f1y.evalf(subs={x: x0, y: y0}) * lr
    # Append to the list to keep track of the points
    xlist.append(x0)
    ylist.append(y0)

xarr = np.array(xlist, dtype='float64')
yarr = np.array(ylist, dtype='float64')
zlist = list(f(xarr, yarr))

# Plot the surface and points
fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='viridis', edgecolor='none')
ax.plot(xlist, ylist, zlist, 'ro', markersize=5, alpha=0.6)
ax.set_title('Gradient Descent')
plt.show()
