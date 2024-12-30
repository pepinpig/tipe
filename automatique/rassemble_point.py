import sys
import math
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


def main():
    if len(sys.argv) != 4:
        print("Usage : python3 script.py fichier1 fichier2 fichier3")
        sys.exit(1)
    
    fichiers_entrants = sys.argv[1:4]
    fichier_sortie =  f'points/donnees/points_3D-{fichiers_entrants[0]}.txt'
    complete_nb_points = f'points/donnees/nb_points-{fichiers_entrants[0]}.txt'
    tous_les_points = []
    for fichier in fichiers_entrants:
        complete_fichier = f'points/donnees/points_reconstruits-{fichier}.txt'
        tous_les_points.extend(lire_points(complete_fichier))
    
    ecrire_points(fichier_sortie, tous_les_points)
    print(f"Fichier '{fichier_sortie}' créé avec {len(tous_les_points)} points.")
    with open(complete_nb_points, "w") as f:
        f.write(str(len(tous_les_points)))

if __name__ == "__main__":
    main()
