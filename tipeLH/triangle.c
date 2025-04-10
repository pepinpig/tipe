#include"triangle.h"

// Produit scalaire pour des vecteurs de dimension 3
double scalaire(double* v1, double* v2) {
    double s = 0;
    for (int i = 0; i < 3; i++) {
        s += v1[i] * v2[i];
    }
    return s;
}
double norme(double* v) {
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
// Addition de deux vecteurs (retourne un nouveau vecteur)
double* add(double* v1, double* v2) {
    double* v3 = malloc(3 * sizeof(double));
    if (!v3) {
        perror("Erreur d'allocation pour add");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++) {
        v3[i] = v1[i] + v2[i];
    }
    return v3;
}

// Addition in-place de deux vecteurs
void addin(double* v1, double* v2) {
    for (int i = 0; i < 3; i++) {
        v1[i] += v2[i];
    }
}

// Soustraction de deux vecteurs (retourne un nouveau vecteur)
double* sub(double* v1, double* v2) {
    double* v3 = malloc(3 * sizeof(double));
    if (!v3) {
        perror("Erreur d'allocation pour sub");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++) {
        v3[i] = v1[i] - v2[i];
    }
    return v3;
}

// Mise à l'échelle d'un vecteur
void scale(double* v1, double s) {
    for (int i = 0; i < 3; i++) {
        v1[i] *= s;
    }
}

// Calcul du barycentre
double* barycentre(int* l, int k, double** points) {
    double* bary = malloc(3 * sizeof(double));
    if (!bary) {
        perror("Erreur d'allocation pour barycentre");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++) {
        bary[i] = 0;
    }
    for (int i = 0; i < k; i++) {
        addin(bary, points[l[i]]);
    }
    scale(bary, 1.0 / k);
    return bary;
}

// Produit vectoriel pour des vecteurs de dimension 3
double* prod(double* v1, double* v2) {
    double* v3 = malloc(3 * sizeof(double));
    if (!v3) {
        perror("Erreur d'allocation pour prod");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 3; i++) {
        v3[i] = v1[(i + 1) % 3] * v2[(i + 2) % 3] - v1[(i + 2) % 3] * v2[(i + 1) % 3];
    }
    return v3;
}

// Test si un vecteur est nul
bool nulv(double* v) {
    for (int i = 0; i < 3; i++) {
        if (v[i] != 0) {
            return false;
        }
    }
    return true;
}

// Calcul du nombre de combinaisons de 3 parmi n
int trois_parmi(int n) {
    return (n <= 2) ? 0 : n * (n - 1) * (n - 2) / 6;
}

// Génère toutes les combinaisons possibles de triangles
int** triangles(int card) {
    int n = trois_parmi(card);
    int** trigs = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        trigs[i] = malloc(3 * sizeof(int));
    }
    int ind = 0;
    for (int i = 0; i < card; i++) {
        for (int j = i + 1; j < card; j++) {
            for (int k = j + 1; k < card; k++) {
                trigs[ind][0] = i;
                trigs[ind][1] = j;
                trigs[ind][2] = k;
                ind++;
            }
        }
    }
    return trigs;
}

// Impression d'un vecteur dans un fichier
void file_print_vect(double* v, FILE* file) {
    fprintf(file, "%f %f %f\n", v[0], v[1], v[2]);
}

// Impression d'un vecteur sur la console
void print_vect(double* v) {
    printf("(%1.3f, %1.3f, %1.3f)\n", v[0], v[1], v[2]);
}

// Destruction des triangles
void destroy_trigs(int** l, int n) {
    for (int i = 0; i < n; i++) {
        free(l[i]);
    }
    free(l);
}

// Destruction des points
void destroy_points(double** l, int n) {
    for (int i = 0; i < n; i++) {
        free(l[i]);
    }
    free(l);
}

bool* keeptrig(int** l, int ntrig, int size, double** point) {
    bool* res = malloc(ntrig * sizeof(bool));
    
    for (int i = 0; i < ntrig; i++) {
        double* v1 = sub(point[l[i][1]], point[l[i][0]]);
        double* v2 = sub(point[l[i][2]], point[l[i][0]]);

        double* n = prod(v1, v2);
        
        if (norme(n) < SEUIL) {
            res[i] = false;
        } else {
            double* bary = barycentre(l[i], 3, point);
            res[i] = true;
            
            int signe = 0;
            for (int j = 0; j < size; j++) {
                double* v = sub(bary, point[j]);
                double s = scalaire(n, v);
                
                if (fabs(s) < SEUIL) {
                    s = 0;
                }
                if (s != 0) {
                    if (signe == 0) {
                        signe = (s > 0) ? 1 : -1;
                    } else if (signe * s < 0) {
                        res[i] = false;
                        break;
                    }
                }
                
                free(v);
            }
            free(bary);
        }
        
        // Libération de la mémoire des vecteurs utilisés pour le calcul
        free(v1);
        free(v2);
        free(n);
    }
    
    return res;
}


// Lecture des points depuis un fichier
double** read_points(char* filename, int count) {
    char complete_fn[256];
    snprintf(complete_fn, sizeof(complete_fn), "points/donnees/%s.txt", filename);
    FILE* file = fopen(complete_fn, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier de points");
        exit(EXIT_FAILURE);
    }
    double** points = malloc(count * sizeof(double*));
    if (!points) {
        perror("Erreur d'allocation pour les points");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i++) {
        points[i] = malloc(3 * sizeof(double));
        if (!points[i]) {
            perror("Erreur d'allocation pour un point");
            exit(EXIT_FAILURE);
        }
        fscanf(file, "%lf %lf %lf", &points[i][0], &points[i][1], &points[i][2]);
    }
    fclose(file);
    return points;
}
