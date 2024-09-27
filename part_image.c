#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stb_image.h"

#define WIDTH 800
#define HEIGHT 600
#define POINTS_REQUIRED 8

typedef struct {
    float x, y;
} Point;

// Variables globales pour stocker les points de correspondance
Point* pointsImage1 = NULL;
Point* pointsImage2 = NULL;
int pointsImage1Count = 0;
int pointsImage2Count = 0;
int pointsImage1Capacity = 0;
int pointsImage2Capacity = 0;

void drawEpipolarLine(float F[3][3], Point p, int imageIndex);
void calculateFundamentalMatrix(float F[3][3]);

void addPoint(Point** pointsArray, int* count, int* capacity, Point p) {
    // Redimensionner le tableau si nécessaire
    if (*count >= *capacity) {
        *capacity = (*capacity == 0) ? 1 : *capacity * 2;
        *pointsArray = (Point*)realloc(*pointsArray, (*capacity) * sizeof(Point));
        if (*pointsArray == NULL) {
            fprintf(stderr, "Erreur de réallocation de mémoire\n");
            exit(1);
        }
    }
    (*pointsArray)[(*count)++] = p;
}

// Fonction pour afficher des images (simple boîte de remplissage comme exemple)
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Dessiner les deux images
    glColor3f(1.0, 1.0, 1.0); // Couleur blanche pour les images
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WIDTH / 2, 0);
        glVertex2f(WIDTH / 2, HEIGHT);
        glVertex2f(0, HEIGHT);
    glEnd();
    
    glBegin(GL_QUADS);
        glVertex2f(WIDTH / 2, 0);
        glVertex2f(WIDTH, 0);
        glVertex2f(WIDTH, HEIGHT);
        glVertex2f(WIDTH / 2, HEIGHT);
    glEnd();
    
    glFlush();
}

void mouseClick(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        Point p = {(float)x, (float)(HEIGHT - y)};
        
        if (x < WIDTH / 2) {
            addPoint(&pointsImage1, &pointsImage1Count, &pointsImage1Capacity, p);
            printf("Point ajouté à l'image 1 : (%.1f, %.1f)\n", p.x, p.y);
        } else {
            p.x -= WIDTH / 2;
            addPoint(&pointsImage2, &pointsImage2Count, &pointsImage2Capacity, p);
            printf("Point ajouté à l'image 2 : (%.1f, %.1f)\n", p.x, p.y);
        }
        
        if (pointsImage1Count >= POINTS_REQUIRED && pointsImage2Count >= POINTS_REQUIRED) {
            // Calculer la matrice fondamentale F
            float F[3][3];
            calculateFundamentalMatrix(F);
            
            // Afficher les droites épipolaires pour toutes les correspondances
            for (int i = 0; i < pointsImage1Count; ++i) {
                drawEpipolarLine(F, pointsImage1[i], 2); // Dessiner sur la deuxième image
                drawEpipolarLine(F, pointsImage2[i], 1); // Dessiner sur la première image
            }
        }
        
        glutPostRedisplay();
    }
}

void calculateFundamentalMatrix(float F[3][3]) {
    // Implémenter l'algorithme pour calculer la matrice fondamentale F ici
    // En utilisant les points de correspondance pointsImage1 et pointsImage2
    // Placeholder simple:
    F[0][0] = F[1][1] = F[2][2] = 1.0f;
    F[0][1] = F[0][2] = F[1][0] = F[1][2] = F[2][0] = F[2][1] = 0.0f;
}

void drawEpipolarLine(float F[3][3], Point p, int imageIndex) {
    // Calculer la droite épipolaire à partir de p et F
    // Placeholder pour le calcul réel de la droite épipolaire
    float a = F[0][0] * p.x + F[0][1] * p.y + F[0][2];
    float b = F[1][0] * p.x + F[1][1] * p.y + F[1][2];
    float c = F[2][0] * p.x + F[2][1] * p.y + F[2][2];
    
    float x0 = 0;
    float y0 = -c / b;
    float x1 = WIDTH / 2;
    float y1 = -(c + a * x1) / b;
    
    if (imageIndex == 2) {
        x0 += WIDTH / 2;
        x1 += WIDTH / 2;
    }
    
    glColor3f(1.0, 0.0, 0.0); // Couleur rouge pour la ligne épipolaire
    glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
    glEnd();
    
    glFlush();
}

void initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Fond noir
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT); // Définir le système de coordonnées
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Matrice fondamentale avec GLUT");
    
    initialize();
    glutDisplayFunc(display);
    glutMouseFunc(mouseClick);
    
    glutMainLoop();
    
    // Libérer la mémoire allouée dynamiquement
    free(pointsImage1);
    free(pointsImage2);
    
    return 0;
}
