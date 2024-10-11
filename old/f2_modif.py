import numpy as np
from matplotlib import pyplot as plt

x = np.arange(0,50,1)
y = np.arange(0,50,1)

# Goal is at (33,40) 
goal = [33,40]

#obstacle is at(25,25)
obstacle = [25,25]
X, Y = np.meshgrid(x,y)

delx = np.zeros_like(X)
dely = np.zeros_like(Y)

#parameters
k1=5
k2=200
r=2
for i in range(len(x)):
  for j in range(len(y)):
    
    # finding the goal distance and obstacle distance
    d_goal = np.sqrt((goal[0]-X[i][j])**2 + ((goal[1]-Y[i][j]))**2)
    d_obstacle = np.sqrt((obstacle[0]-X[i][j])**2 + (obstacle[1]-Y[i][j])**2)

    #finding theta correspoding to the goal and obstacle 
    theta_goal= np.arctan2(goal[1] - Y[i][j], goal[0]  - X[i][j])
    theta_obstacle = np.arctan2(obstacle[1] - Y[i][j], obstacle[0]  - X[i][j])
    delx_g = 0
    dely_g =0
    delx_o = 0
    dely_o = 0
    if d_goal >2:
      delx_g = k1*(goal[0]-X[i][j]/d_goal)
      dely_g = k1*(goal[1]-Y[i][j]/d_goal)
    if d_obstacle >1:
      if d_obstacle >2:
      delx_o = -(k2*(obstacle[0]-X[i][j]))/(d_obstacle)**2
      dely_o = -(k2*(obstacle[1]-Y[i][j]))/(d_obstacle)**2
    delx[i][j] = delx_g + delx_o
    dely[i][j] = dely_g + dely_o
    print(delx_o,delx_g)

fig, ax = plt.subplots(figsize=(10,10))
ax.quiver(X, Y, delx, dely)
fig, ax = plt.subplots(figsize=(10,10))

seek_points = np.array([[0,0]]) 
ax.streamplot(X,Y,delx,dely, start_points=seek_points)
ax.quiver(X, Y, delx, dely)
ax.add_patch(plt.Circle((25, 25), r, color='y'))
ax.add_patch(plt.Circle((33, 40), r, color='m'))

ax.annotate("Obstacle", xy=(25, 25), fontsize=8, ha="center")
ax.annotate("Goal", xy=(33, 40), fontsize=10, ha="center")

ax.set_title('Path taken by the Robot ')

plt.show()