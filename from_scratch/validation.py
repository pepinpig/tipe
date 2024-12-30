import numpy as np

def lire_matrice(fichier):
    """Lit une matrice à partir d'un fichier texte."""
    with open(fichier, 'r') as f:
        # Ignorer la première ligne
        next(f)    
        # Lire le reste du fichier comme une matrice
        matrice = np.loadtxt(f, dtype=float)
    return matrice


def ajuster_signe(p):
    if p[2] < 0:
        p = -p
    return p

def construire_matrice_A(P1, P2, u1, v1, u2, v2):
    """Construit la matrice A pour un point donné."""
    A = np.zeros((4, 4))
    A[0] = u1 * P1[2] - P1[0]
    A[1] = v1 * P1[2] - P1[1]
    A[2] = u2 * P2[2] - P2[0]
    A[3] = v2 * P2[2] - P2[1]
    return A

def trianguler_points(P1, P2, points_image1, points_image2):
    """
    Triangule les points 3D à partir des correspondances dans deux images.
    :param P1: Matrice de projection de la première caméra (3x4).
    :param P2: Matrice de projection de la deuxième caméra (3x4).
    :param points_image1: Points dans la première image (Nx2).
    :param points_image2: Points dans la deuxième image (Nx2).
    :return: Points 3D reconstruits (Nx3).
    """
    points_3D = []

    for (u1, v1), (u2, v2) in zip(points_image1, points_image2):
        A = construire_matrice_A(P1, P2, u1, v1, u2, v2)
        _, S, Vt = np.linalg.svd(A)
        X_homog = Vt[-1]
        X = X_homog[:3] / X_homog[3]
        X=ajuster_signe(X)
        points_3D.append(X)

    return np.array(points_3D)

def test_triangulation():
    # Matrices de projection
    P1 = lire_matrice("P-img1.txt")
    P2 = lire_matrice("P-img2.txt")

    # Points 2D dans les images
    points_image1 = np.array([[0.0, 0.0], [0.2, 0.0], [0.0, 0.2], [0.2, 0.2]])
    points_image2 = np.array([[0.1, 0.0], [0.3, 0.0], [0.1, 0.2], [0.3, 0.2]])

    # Points 3D attendus (pour validation)
    points_3D_attendus = np.array([[0.0, 0.0, 5.0],
                                    [1.0, 0.0, 5.0],
                                    [0.0, 1.0, 5.0],
                                    [1.0, 1.0, 5.0]])

    # Triangulation
    points_3D_reconstruits = trianguler_points(P1, P2, points_image1, points_image2)

    print("Points 3D reconstruits :")
    print(points_3D_reconstruits)

    # Validation des résultats
    for i, (reconstruit, attendu) in enumerate(zip(points_3D_reconstruits, points_3D_attendus)):
        erreur = np.linalg.norm(reconstruit - attendu)
        if erreur < 1e-6:
            print(f"Point {i} correct.")
        else:
            print(f"Point {i} incorrect : erreur = {erreur}")

if __name__ == "__main__":
    test_triangulation()