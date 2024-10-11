import numpy as np
import matplotlib.pyplot as plt

# Données
L = np.array([40, 28, 12, 8, 4, 2])
B = np.array([1.2, 1.2, 1.1, 1.0, 0.8, 0.2])

plt.plot(L, B, 'r', label='Données')
plt.plot(L, B, 'xb')
plt.grid(True)
plt.ylabel('B (T)')
plt.xlabel('L (cm)')
plt.axhline(y=1.2 - 0.05 * 1.2, color='g', linestyle='--', label='-5% de 1.2 T')
plt.title("Champ magnétique en fonction de la longueur")
plt.legend()
plt.show()
