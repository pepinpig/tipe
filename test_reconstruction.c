#include "camera_calibration.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nom_image1> <nom_image2>\n", argv[0]);
        return 1;
    }

    char* image_name1 = argv[1];
    char* image_name2 = argv[2];

    char file_points1[256], file_points2[256];
    snprintf(file_points1, sizeof(file_points1), "points/donnees/points_ap_%s.txt", image_name1);
    snprintf(file_points2, sizeof(file_points2), "points/donnees/points_ap_%s.txt", image_name2);

    char p_file1[256], p_file2[256];
    snprintf(p_file1, sizeof(p_file1), "P-%s.txt", image_name1);
    snprintf(p_file2, sizeof(p_file2), "P-%s.txt", image_name2);

    matrice* P1 = matrice_nulle(3, 4);
    matrice* P2 = matrice_nulle(3, 4);
    read_matrice_from_file(P1, p_file1);
    read_matrice_from_file(P2, p_file2);

    FILE* f1 = fopen(file_points1, "r");
    FILE* f2 = fopen(file_points2, "r");
    if (!f1 || !f2) {
        fprintf(stderr, "Erreur ouverture fichiers points.\n");
        return 1;
    }

    char output_file[256];
    snprintf(output_file, sizeof(output_file), "points/donnees/points_3d_%s.txt", image_name1);
    FILE* output = fopen(output_file, "w");
    assert(output != NULL);

    printf("Reconstruction des points 3D ...\n");
    double u1, v1, u2, v2;
    while (fscanf(f1, "%lf %lf", &u1, &v1) == 2 && fscanf(f2, "%lf %lf", &u2, &v2) == 2) {
        if ((int)u1 == -1 && (int)v1 == -1) continue;
        if ((int)u2 == -1 && (int)v2 == -1) continue;

        matrice* A = matrice_nulle(4, 4);
        for (int j = 0; j < 4; ++j) {
            A->mat[0][j] = u1 * P1->mat[2][j] - P1->mat[0][j];
            A->mat[1][j] = v1 * P1->mat[2][j] - P1->mat[1][j];
            A->mat[2][j] = u2 * P2->mat[2][j] - P2->mat[0][j];
            A->mat[3][j] = v2 * P2->mat[2][j] - P2->mat[1][j];
        }

        matrice* S = matrice_nulle(A->m, A->n);
        matrice* V = matrice_nulle(A->n, A->n);
        matrice* U = matrice_nulle(A->m, A->m);
        qr_algorithm_SVD(A, U, S, V);

        int index_min = S->n - 1;
        matrice* p = matrice_nulle(V->m, 1);
        for (int i = 0; i < V->m; i++) {
            p->mat[i][0] = V->mat[i][index_min];
        }

        for (int i = 0; i < 3; ++i) {
            p->mat[i][0] /= p->mat[3][0];
        }

        fprintf(output, "%f %f %f\n", p->mat[0][0], p->mat[1][0], p->mat[2][0]);

        // Libérer les matrices temporaires
        free_matrice(A); free_matrice(S); free_matrice(V); free_matrice(U); free_matrice(p);
    }

    fclose(f1);
    fclose(f2);
    fclose(output);
    printf("Points reconstruits écrits dans : %s\n", output_file);
    char command[256];
    snprintf(command, sizeof(command), "python3 plot_points_3D.py points_3d_%s", image_name1);
    system(command);
    free_matrice(P1);
    free_matrice(P2);
    return 0;
}