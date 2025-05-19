#include"ransac.h"

double point_to_line_dist(double x, double y, double a, double b, double c) {
    return fabs(a*x + b*y + c) / sqrt(a*a + b*b);
}

void detect_lines_and_extremities(matrice* points) {
    srand(time(NULL));

    printf("Démarrage de RANSAC sur %d points\n", points->n);

    for (int iter = 0; iter < RANSAC_ITER; ++iter) {
        int i1 = rand() % points->n;
        int i2 = rand() % points->n;
        if (i1 == i2) continue;

        double x1 = points->mat[i1][0], y1 = points->mat[i1][1];
        double x2 = points->mat[i2][0], y2 = points->mat[i2][1];

        if ((x1 == -1 && y1 == -1) || (x2 == -1 && y2 == -1)) continue;

        double a = y2 - y1;
        double b = x1 - x2;
        double c = x2 * y1 - x1 * y2;

        int inlier_count = 0;
        int* inliers = malloc(sizeof(int) * points->n);

        for (int i = 0; i < points->n; ++i) {
            double x = points->mat[i][0];
            double y = points->mat[i][1];
            if (x == -1 && y == -1) continue;

            if (point_to_line_dist(x, y, a, b, c) < DIST_THRESHOLD) {
                inliers[inlier_count++] = i;
            }
        }

        if (inlier_count >= MIN_INLIERS) {
            double min_proj = 1e9, max_proj = -1e9;
            double end1[2], end2[2];

            for (int i = 0; i < inlier_count; ++i) {
                int idx = inliers[i];
                double x = points->mat[idx][0];
                double y = points->mat[idx][1];
                double proj = x * a + y * b;

                if (proj < min_proj) {
                    min_proj = proj;
                    end1[0] = x;
                    end1[1] = y;
                }
                if (proj > max_proj) {
                    max_proj = proj;
                    end2[0] = x;
                    end2[1] = y;
                }
            }
            for (int i = 0; i < inlier_count; ++i) {
                int idx = inliers[i];
                double x = points->mat[idx][0];
                double y = points->mat[idx][1];

                if (!((x == end1[0] && y == end1[1]) || (x == end2[0] && y == end2[1]))) {
                    points->mat[idx][0] = -1;
                    points->mat[idx][1] = -1;
                }
            }

        }
        free(inliers);
    }
}

