#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

// Variables globales pour les textures
GLuint texture1, texture2;

// Fonction pour vérifier les erreurs OpenGL
void checkOpenGLErrors(const char *location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL error %d at %s\n", err, location);
    }
}

void loadTextures() {
    int width, height, nrChannels;
    unsigned char *data;

    // Charger la première image
    data = stbi_load("image1.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        checkOpenGLErrors("loading texture1");
    } else {
        fprintf(stderr, "Failed to load image1.jpg\n");
    }

    // Charger la deuxième image
    data = stbi_load("image2.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
        checkOpenGLErrors("loading texture2");
    } else {
        fprintf(stderr, "Failed to load image2.jpg\n");
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Dessiner la première image
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(WIDTH / 2, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(WIDTH / 2, HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, HEIGHT);
    glEnd();

    // Dessiner la deuxième image
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(WIDTH / 2, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(WIDTH, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(WIDTH, HEIGHT);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(WIDTH / 2, HEIGHT);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glFlush();
    checkOpenGLErrors("display");
}

void initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    // Charger les textures
    loadTextures();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Affichage d'images avec GLUT");

    initialize();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
