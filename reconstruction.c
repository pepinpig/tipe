#include "reconstruction.h"

int reconstruction4(const char* in1, const char* in2,  const char* in3,  const char* in4,  const char* in5,const char* in6, const char* in7,const char* in8, matrice** mat){
    matrice* mat1;
    matrice* mat2; 
    matrice* mat3; 
    matrice* mat4; 
    int nbp=reconstruction1(in1, in2, &mat1);
    nbp+=reconstruction1(in3, in4, &mat2);
    nbp+=reconstruction1(in5, in6, &mat3);
    nbp+=reconstruction1(in7, in8, &mat4);
    *mat=concatenationv3(concatenationv2(mat1, mat2),mat3,mat4);
    return nbp;
}

int reconstruction1(const  char* image_name1, const char* image_name2,  matrice** mat){
    int points_ecrits = 0;
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
    int nb_points =reconstruction1_aux(P1,P2, file_points1,file_points2, mat, image_name1);
    return nb_points;
    free_matrice(P1);
    free_matrice(P2);
}

bool filtre (matrice* p){
    if ((fabs(p->mat[0][0])>1.25)||(fabs(p->mat[1][0]))>1.25||(fabs(p->mat[2][0])>1.25)){
        return false;
    }
    return true;
}


int reconstruction1_aux(matrice* P1, matrice* P2,const char* file_points1, const char* file_points2, matrice** mat, const char* image_name1) {
    int points_ecrits=0;
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

        if (((p->mat[2][0])>1)&&(filtre(p))){
            fprintf(output, "%f %f %f\n", p->mat[0][0], p->mat[1][0], p->mat[2][0]);
            points_ecrits++;
        }

        // Libérer les matrices temporaires
        free_matrice(A); free_matrice(S); free_matrice(V); free_matrice(U); free_matrice(p);
    }
    fclose(output);
    printf("pointsecrits : %d", points_ecrits);
    *mat=matrice_nulle(points_ecrits, 3);
    snprintf(output_file, sizeof(output_file), "points_3d_%s.txt", image_name1);
    read_matrice_from_file(*mat,output_file);
    print_matrice(*mat);
    //printf("Points reconstruits écrits dans : %s\n", output_file);
    fclose(f1);
    fclose(f2);
    return points_ecrits;
}

