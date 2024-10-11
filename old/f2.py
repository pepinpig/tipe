import numpy as np
from matplotlib import pyplot as plt

x = np.arange(-0,50,1)
y = np.arange(-0,50,1)

# Goal is at (40,40) 
goal = [40,40]

#obstacle is at(25,25)
obstacle = [25,25]
X, Y = np.meshgrid(x,y)

delx = np.zeros_like(X)
dely = np.zeros_like(Y)

"""
    Inside the nested loop, distance from each point to the goal and ostacle is 
    calculated, Similarly angles are calculated. Then I simply used the formula give and 
    superimposed it to the Goals potential field.Also
    α = 50
    β = 50
    s = 15
    r = 2
"""
s = 15
r=2
for i in range(len(x)):
  for j in range(len(y)):
    
    # finding the goal distance and obstacle distance
    d_goal = np.sqrt((goal[0]-X[i][j])**2 + ((goal[1]-Y[i][j]))**2)
    d_obstacle = np.sqrt((obstacle[0]-X[i][j])**2 + (obstacle[1]-Y[i][j])**2)
    #print(f"{i} and {j}")

    #finding theta correspoding to the goal and obstacle 
    theta_goal= np.arctan2(goal[1] - Y[i][j], goal[0]  - X[i][j])
    theta_obstacle = np.arctan2(obstacle[1] - Y[i][j], obstacle[0]  - X[i][j])

    if d_obstacle < r:
      delx[i][j] = np.sign(np.cos(theta_obstacle)) +0
      dely[i][j] = np.sign(np.cos(theta_obstacle))  +0
    elif d_obstacle>r+s:
      delx[i][j] = 0 +(50 * s *np.cos(theta_obstacle))
      dely[i][j] = 0 + (50 * s *np.sin(theta_goal))
    elif d_obstacle<r+s :
      delx[i][j] = -120 *(s+r-d_obstacle)* np.cos(theta_obstacle)
      dely[i][j] = -120 * (s+r-d_obstacle)*  np.sin(theta_obstacle) 
    if d_goal <r+s:
      if delx[i][j] != 0:
        delx[i][j]  += (50 * (d_goal-r) *np.cos(theta_goal))
        dely[i][j]  += (50 * (d_goal-r) *np.sin(theta_goal))
      else:
        delx[i][j]  = (50 * (d_goal-r) *np.cos(theta_goal))
        dely[i][j]  = (50 * (d_goal-r) *np.sin(theta_goal))
        
    if d_goal>r+s:
      if delx[i][j] != 0:
        delx[i][j] += 50* s *np.cos(theta_goal)
        dely[i][j] += 50* s *np.sin(theta_goal)
      else:
        delx[i][j] = 50* s *np.cos(theta_goal)
        dely[i][j] = 50* s *np.sin(theta_goal)

fig, ax = plt.subplots(figsize=(10,10))
ax.quiver(X, Y, delx, dely)
fig, ax = plt.subplots(figsize=(10,10))

seek_points = np.array([[0,0]]) 
ax.streamplot(X,Y,delx,dely, start_points=seek_points)
ax.quiver(X, Y, delx, dely)
ax.add_patch(plt.Circle((25, 25), r, color='y'))
ax.add_patch(plt.Circle((33, 40), r, color='m'))

ax.annotate("Obstacle", xy=(25, 25), fontsize=8, ha="center")
ax.annotate("Objectif", xy=(33, 40), fontsize=10, ha="center")

ax.set_title('Chemin pris par le robot')

plt.show() 