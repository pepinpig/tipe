#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "N.h"
#include "camera_calibration9.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "N.h"
#include "camera_calibration9.h"


// Fonction pour enregistrer les couleurs dans un fichier
void print_couleur(char* c1, char* c2, char* c3, char* image_name) {
    char complete_fn[128];
    snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", image_name);

    FILE* file = fopen(complete_fn, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", c1);
    fprintf(file, "%s\n", c2);
    fprintf(file, "%s\n", c3);

    fclose(file);
}
bool couleur_valide(char* c){
    if (!strcmp(c,"BO")){
        return true;
    }
    if (!strcmp(c,"OV")){
        return true;
    }
    if (!strcmp(c,"VR")){
        return true;
    }
    if (!strcmp(c,"RB")){
        return true;
    }
    return false;#include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include "N.h"
    #include "camera_calibration9.h"
    
    
    // Fonction pour enregistrer les couleurs dans un fichier
    void print_couleur(char* c1, char* c2, char* c3, char* image_name) {
        char complete_fn[128];
        snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", image_name);
    
        FILE* file = fopen(complete_fn, "w");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
    
        fprintf(file, "%s\n", c1);
        fprintf(file, "%s\n", c2);
        fprintf(file, "%s\n", c3);
    
        fclose(file);
    }
    bool couleur_valide(char* c){
        if (!strcmp(c,"BO")){
            return true;
        }
        if (!strcmp(c,"OV")){
            return true;
        }
        if (!strcmp(c,"VR")){
            return true;
        }
        if (!strcmp(c,"RB")){
            return true;
        }
        return false;
    }
    
    // Fonction pour lire les couleurs à partir d'un fichier
    void read_couleur(char* c1, char* c2, char* c3, char* image_name) {
        char complete_fn[128];
        snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", image_name);
    
        FILE* file = fopen(complete_fn, "r");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }
    
        if (fgets(c1, 4, file) == NULL) perror("Erreur lecture c1");
        if (fgets(c2, 4, file) == NULL) perror("Erreur lecture c2");
        if (fgets(c3, 4, file) == NULL) perror("Erreur lecture c3");
    
}

// Fonction pour lire les couleurs à partir d'un fichier
void read_couleur(char* c1, char* c2, char* c3, char* image_name) {
    char complete_fn[128];
    snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", image_name);

    FILE* file = fopen(complete_fn, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    if (fgets(c1, 4, file) == NULL) perror("Erreur lecture c1");
    if (fgets(c2, 4, file) == NULL) perror("Erreur lecture c2");
    if (fgets(c3, 4, file) == NULL) perror("Erreur lecture c3");

    fclose(file);
}

int main(int argc, char* argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nom_image> <o/N:calibration> \n", argv[0]);
        return 1;
    }
    char image=argv[1];
    char calibrage =argv[2];
    char command[256];

    // Vérification de l'existence des images
    for (int i = 0; i < 6; ++i) {
        char file_image[128];
        snprintf(file_image, sizeof(file_image), "points/images/%s%d.%s", image_name, i, extension);
        printf("Lecture %s%d.%s\n", image_name, i, extension);
        FILE *file = fopen(file_image, "r");
        if (file == NULL) {
            perror("L'image n'a pas été trouvée");
            return 1;
        }
        fclose(file);
    }
    //Repérage des points de calibrations avec python
    if (calibrage == 'O') {
        snprintf(command, sizeof(command), "python3 pre_calib.py %s", image);
        system(command);
    }
    read_couleur(couleurs_p1, couleurs_p2, couleurs_p3, image);

    // Calcul des paramètres intrinsèques (Calibration)
    char image_name_temp[128];
    char couleur_temp[64];
    for (int i = 0; i < 6; ++i) {
        snprintf(image_name_temp sizeof(image_name_temp), "%s%d", image_name, i);
        snprintf(couleur_temp sizeof(couleur_temp), "%s", image_name, i, i < 2 ? couleurs_p1 : (i < 4 ? couleurs_p2 : couleurs_p3));
        camera_calibration_main(image_name_temp,couleur_temp);
    }
    // Sélection des points et appariement

    //A implementer...

    for (int i = 0; i < 6; i += 2) {
        snprintf(command, sizeof(command), "./r %s%d %s%d %s", image_name, i, image_name, i + 1, i < 2 ? couleurs_p1 : (i < 4 ? couleurs_p2 : couleurs_p3));
        image_name
    }

    //Affichage temporaire des resultats
    // Affichage des points par fichier et génération des résultats finaux
    snprintf(command, sizeof(command), "python3 rassemble_point.py %s0 %s2 %s4", image_name, image_name, image_name);
    system(command);
    snprintf(command, sizeof(command), "python3 plot_points.py points_reconstruits-%s0 points_reconstruits-%s2 points_reconstruits-%s4", image_name,image_name,image_name);
    snprintf(command, sizeof(command), "gcc triangle.c -o t -lm");
    system(command);
    snprintf(command, sizeof(command), "./t %s0", image_name);
    system(command);
    snprintf(command, sizeof(command), "python3 plt_triangles.py %s", image_name);
    system(command);
    return 0;
}
