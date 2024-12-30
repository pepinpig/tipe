import numpy as np

def lire_matrice(fichier):
    """Lit une matrice à partir d'un fichier texte."""
    with open(fichier, 'r') as f:
        matrice = np.loadtxt(f, dtype=float)
    return matrice

def lire_points_reels(fichier):
    """
    Lit des points réels à partir d'un fichier texte.
    Si les points ne sont pas en coordonnées homogènes (3 dimensions),
    ajoute une composante homogène (W=1).
    """
    with open(fichier, 'r') as f:
        points = np.loadtxt(f, dtype=float)
    
    # Si les points ont 3 dimensions, ajoute une colonne de 1
    if points.shape[1] == 3:
        ones = np.ones((points.shape[0], 1))  # Colonne de 1
        points = np.hstack((points, ones))  # Concatène les colonnes
    return points

def tester_matrice_projection(matrice_projection, points_reels):
    """
    Applique la matrice de projection aux points réels et affiche les résultats.
    :param matrice_projection: Matrice de projection 3x4.
    :param points_reels: Points réels en coordonnées homogènes (Nx4).
    """
    print("Validation de la matrice de projection :")
    print(matrice_projection)
    print("\nRésultats du produit PX :")
    for i, point in enumerate(points_reels):
        print(f"Point réel {i + 1} avant projection : {point}")
        # Calcul du produit PX
        projete = np.dot(matrice_projection, point)
        print(f"Point projeté homogène (non normalisé) : {projete}")
        # Normalisation des coordonnées homogènes
        if projete[-1] != 0:
            projete /= projete[-1]
        print(f"Point projeté (normalisé) : {projete}")
        print("-" * 30)


def main():
    # Charger les fichiers
    fichier_matrice = input("Entrez le chemin du fichier de la matrice de projection : ")
    fichier_points = input("Entrez le chemin du fichier des points réels : ")

    # Lire les données
    matrice_projection = lire_matrice(fichier_matrice)
    points_reels = lire_points_reels(fichier_points)

    # Tester la matrice de projection
    tester_matrice_projection(matrice_projection, points_reels)

if __name__ == "__main__":
    main()
