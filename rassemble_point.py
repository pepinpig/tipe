import sys
import math

# Tolérance pour considérer que deux points sont "très proches"
TOLERANCE = 1e-1

def distance(p1, p2):
    """Calcul de la distance euclidienne entre deux points (x, y, z)."""
    return math.sqrt((p2[0] - p1[0]) ** 2 + (p2[1] - p1[1]) ** 2 + (p2[2] - p1[2]) ** 2)

def lire_points(fichier):
    """Lit les points 3D d'un fichier et retourne une liste de tuples (x, y, z)."""
    points = []
    with open(fichier, 'r') as f:
        for ligne in f:
            try:
                x, y, z = map(float, ligne.strip().split())
                if not math.isnan(x):
                    points.append((x, y, z))
            except ValueError:
                print(f"Erreur en lisant une ligne dans {fichier}: {ligne}")
    if not points:
        print(f"Aucun point 3D trouvé dans {fichier}")
    return points

def ecrire_points(fichier_sortie, points):
    """Écrit les coordonnées des points dans un fichier."""
    with open(fichier_sortie, 'w') as f:
        for x, y, z in points:
            f.write(f"{x} {y} {z}\n")

def ajouter_si_unique(points_existants, nouveau_point):
    """Ajoute un point à la liste seulement s'il n'est pas trop proche d'un autre point."""
    for point in points_existants:
        if distance(point, nouveau_point) < TOLERANCE:
            return False  # Le point est trop proche, on ne l'ajoute pas
    points_existants.append(nouveau_point)
    return True

def main():
    if len(sys.argv) != 4:
        print("Usage : python3 script.py fichier1 fichier2 fichier3")
        sys.exit(1)
    
    fichiers_entrants = sys.argv[1:4]
    fichier_sortie =  f'points/donnees/points_3D-{fichiers_entrants[0]}.txt'
    complete_nb_points = f'points/donnees/nb_points-{fichiers_entrants[0]}.txt'
    
    # Liste pour stocker tous les points sans doublons
    tous_les_points = []
    
    for fichier in fichiers_entrants:
        complete_fichier = f'points/donnees/points_reconstruits-{fichier}.txt'
        points = lire_points(complete_fichier)
        
        for point in points:
            # Ajouter le point seulement s'il n'est pas trop proche d'un autre point
            ajouter_si_unique(tous_les_points, point)
    
    ecrire_points(fichier_sortie, tous_les_points)
    print(f"Fichier '{fichier_sortie}' créé avec {len(tous_les_points)} points.")
    
    with open(complete_nb_points, "w") as f:
        f.write(str(len(tous_les_points)))

if __name__ == "__main__":
    main()
