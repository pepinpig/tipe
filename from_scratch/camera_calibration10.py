import numpy as np
from scipy.linalg import svd, inv, qr
import cv2

def construct_A(X, Y, Z, u, v):
    N = len(X)
    A = np.zeros((2 * N, 12))

    for i in range(N):
        row1 = 2 * i
        row2 = 2 * i + 1

        # First row: lambda * u = p11 * X + p12 * Y + p13 * Z + p14
        A[row1, 0:4] = [X[i], Y[i], Z[i], 1]
        A[row1, 8:] = [-u[i] * X[i], -u[i] * Y[i], -u[i] * Z[i], -u[i]]

        # Second row: lambda * v = p21 * X + p22 * Y + p23 * Z + p24
        A[row2, 4:8] = [X[i], Y[i], Z[i], 1]
        A[row2, 8:] = [-v[i] * X[i], -v[i] * Y[i], -v[i] * Z[i], -v[i]]

    return A


def camera_calibration_resolution(A):
    # Effectuer la décomposition en valeurs singulières (SVD)
    U, S, Vt = svd(A)
    
    # Le vecteur correspondant à la plus petite valeur singulière est le dernier vecteur de Vt
    p = Vt[-1]  # Dernière ligne de Vt
    
    # Redimensionner le vecteur p en une matrice 3x4
    P = p.reshape((3, 4))

    # Step 2: Decompose P into intrinsic (K), rotation (R), and translation (T)
    P_extract = P[:, :3]
    P_inv = inv(P_extract)
    Q, R = qr(P_inv)

    K = inv(R)
    K /= K[2, 2]  # Normalize K such that K[2, 2] = 1
    R_inv = inv(Q)
    T = np.dot(np.linalg.inv(K), P[:, 3])

    return P, K, R_inv, T

def load_points(filename):
    return np.loadtxt(filename)

def save_matrix_to_file(matrix, name, image_name):
    filename = f"{name}-{image_name}.txt"
    np.savetxt(filename, matrix, fmt='%.6f')
    print(f"Matrix {name} saved to {filename}")

def test_with_opencv(X, Y, Z, u, v, image_name, P_custom):
    # Convert points to required OpenCV format
    object_points = np.column_stack((X, Y, Z)).astype(np.float32).reshape(-1, 1, 3)
    image_points = np.column_stack((u, v)).astype(np.float32).reshape(-1, 1, 2)

    # Estimate intrinsic matrix (initial guess)
    K_init = np.eye(3, dtype=np.float32)
    K_init[0, 0] = K_init[1, 1] = 1000  # Focal length estimate
    K_init[0, 2] = 320  # Principal point x-coordinate
    K_init[1, 2] = 240  # Principal point y-coordinate

    # Run OpenCV calibration
    success, rvec, tvec = cv2.solvePnP(object_points, image_points, K_init, None)

    if not success:
        print("OpenCV solvePnP failed.")
        return

    # Convert rotation vector to rotation matrix
    R, _ = cv2.Rodrigues(rvec)

    # Construct P
    P_opencv = K_init @ np.hstack((R, tvec))

    # Save the OpenCV results
    save_matrix_to_file(P_opencv, "P_opencv", image_name)
    print("Matrice de projection calculée avec OpenCV :\n", P_opencv)

    # Compare with your calculated P
    print("\nComparaison avec votre méthode :")
    print("Différence entre P_opencv et P_custom (norme) :")
    print(np.linalg.norm(P_opencv - P_custom))

def main(image_name):
    points_image_file = f"points/points_{image_name}.txt"
    points_reel_file = f"points/points_reel{image_name}.txt"

    # Load points
    u, v = load_points(points_image_file).T
    X, Y, Z = load_points(points_reel_file).T

    # Construct matrix A
    A = construct_A(X, Y, Z, u, v)
    print("Matrix A:")
    print(A)
    save_matrix_to_file(A, "A", image_name)

    # Perform camera calibration
    P, K, R, T = camera_calibration_resolution(A)
    save_matrix_to_file(P, "P", image_name)
    save_matrix_to_file(K, "K", image_name)
    save_matrix_to_file(R, "R", image_name)
    save_matrix_to_file(T, "T", image_name)

    # Test with OpenCV
    test_with_opencv(X, Y, Z, u, v, image_name, P)

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python script.py <image_name>")
        sys.exit(1)

    image_name = sys.argv[1]
    main(image_name)
