import numpy as np
import os

N = 100  # Nombre de points à reconstruire, à ajuster selon le cas

def load_all_points_images(filename):
    # Charge les coordonnées des points d'image depuis un fichier
    data = np.loadtxt(filename)
    u = data[:, 0]  # Première colonne
    v = data[:, 1]  # Deuxième colonne
    return u, v

def load_all_points_reels(filename):
    # Charge les coordonnées des points réels depuis un fichier
    data = np.loadtxt(filename)
    X = data[:, 0]  # Première colonne
    Y = data[:, 1]  # Deuxième colonne
    Z = data[:, 2]  # Troisième colonne
    return X, Y, Z

def lire_matrice(filename):
    # Charge une matrice depuis un fichier
    return np.loadtxt(filename)

def main(image_name1, image_name2):
    # Vérifier les arguments
    if len(image_name1) == 0 or len(image_name2) == 0:
        print(f"Usage: python {__file__} <nom_image1> <nom_image2>")
        return 1

    # Charge les coordonnées des points
    points_image_file1 = f"points/points_{image_name1}.txt"
    points_reel_file1 = f"points/points_reel{image_name1}.txt"
    points_image_file2 = f"points/points_{image_name2}.txt"
    points_reel_file2 = f"points/points_reel{image_name2}.txt"
    
    print("Lecture des points ...")
    u1, v1 = load_all_points_images(points_image_file1)
    X1, Y1, Z1 = load_all_points_reels(points_reel_file1)
    
    u2, v2 = load_all_points_images(points_image_file2)
    X2, Y2, Z2 = load_all_points_reels(points_reel_file2)

    # Charger les matrices P1 et P2
    p_file1 = f"P-{image_name1}.txt"
    p_file2 = f"P-{image_name2}.txt"
    print("Lecture des matrices de projection ...")
    P1, P2 = lire_matrice(p_file1), lire_matrice(p_file2)

    # Préparer le fichier de sortie
    output_file = "points/points_reconstruit.txt"
    output = open(output_file, 'w')
    if not output:
        print(f"Erreur lors de l'ouverture du fichier de sortie: {output_file}")
        return 1

    print("Reconstruction des points 3D ...")
    for i in range(N):
        # Construire la matrice pour le système d'équations
        A = np.zeros((4, 4))
        for j in range(4):
            A[0][j] = u1[i] * P1[2][j] - P1[0][j]
            A[1][j] = v1[i] * P1[2][j] - P1[1][j]
            A[2][j] = u2[i] * P2[2][j] - P2[0][j]
            A[3][j] = v2[i] * P2[2][j] - P2[1][j]

        print("A")
        print(A)

        # Calculer la décomposition SVD de A
        U, S, Vt = np.linalg.svd(A)
        
        # Trouver le vecteur singulier droit associé à la plus petite valeur singulière
        min_sigma = S[-1]  # La plus petite valeur singulière est le dernier élément de S
        p = Vt[-1]  # Le vecteur propre associé à la plus petite valeur singulière
        print(f"Min Sigma: {min_sigma}")
        print(f"Vecteur propre droit associé à la plus petite valeur singulière : {p}")

        # Reconstruction du point 3D
        print(f"Point {i} reconstruit : ({p[0]}, {p[1]}, {p[2]})")
        output.write(f"{p[0]} {p[1]} {p[2]}\n")

        print("Matrice produit:")
        print(np.dot(A, p))

    output.close()
    print(f"Points reconstruits écrits dans : {output_file}")

if __name__ == "__main__":
    image_name1 = "3"  # Remplacer par le nom réel de l'image
    image_name2 = "5"  # Remplacer par le nom réel de l'image
    main(image_name1, image_name2)
