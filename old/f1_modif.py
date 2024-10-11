import numpy as np
from matplotlib import pyplot as plt
# creating two evenly spaced array with ranging from 
# -10 to 10
x = np.arange(-10,10,1)
y = np.arange(-10,10,1)

# Creating the meshgrid 
X, Y = np.meshgrid(x,y)

#Creating delx and dely array
delx = np.zeros_like(X)
dely = np.zeros_like(Y)
r=2
k=5

for i in range(len(x)):
  for j in range(len(y)):
    
    # calculating the distance asumming the goal is at Origin
    d= np.sqrt(X[i][j]**2 + Y[i][j]**2)
    #print(f"{i} and {j}")

    # 
    if d< 2:
      delx[i][j] = 0
      dely[i][j] =0
    else:
      delx[10-i][10-j] = -k*X[i][j]*d
      dely[10-i][10-j] = -k*Y[i][j]*d

fig, ax = plt.subplots(figsize = (10,10))
ax.quiver(X, Y, delx, dely)
ax.add_patch(plt.Circle((0, 0), 2, color='b'))
ax.annotate("Objectif", xy=(0, 0), fontsize=20, ha="center")
ax.set_title("Champ de potentiel attractif généré par l'objectif")
plt.show() 