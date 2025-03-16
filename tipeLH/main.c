#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "N.h"

// Fonction pour enregistrer les couleurs dans un fichier
void print_couleur(char* c1, char* c2, char* c3, char* base_nom) {
    char complete_fn[128];
    snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", base_nom);

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
void read_couleur(char* c1, char* c2, char* c3, char* base_nom) {
    char complete_fn[128];
    snprintf(complete_fn, sizeof(complete_fn), "points/donnees/couleurs_%s.txt", base_nom);

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
    char caractere;
    char calibrage = 'N';
    char selection_point = 'N';

    // Introduction au programme
    printf("Voulez-vous suivre le tutoriel complet? (O/N) ...\n");
    scanf(" %c", &caractere);
    if (caractere == 'O') {
        calibrage = 'O';
        selection_point = 'O';
        printf("Bienvenue dans ce programme de reconstruction 3D.\n");
        getchar(); // Pause interactive
        printf("Pour commencer, il vous faudra prendre des photos de votre objet avec quelques contraintes que nous allons préciser.\n");
        getchar();
        printf("Nous utilisons comme mire de calibrage un rubik's cube, il vous faudra donc le positionner sur les photos, face blanche vers le haut...\n");
        getchar();
        printf("Afin de reconstruire l'objet en entier, il est conseillé d'utiliser 6 photos.\n");
        getchar();
        printf("Dans tous les cas, chaque paire d'image (images successives) doivent avoir un point de vue proche.\n");
        getchar();
        printf("Par exemple, sur les deux premières images (0 et 1), les faces bleue et orange sont visibles.\n");
        getchar();
        printf("Une fois les 6 photos réalisées, placez-les dans le dossier /points/images et renommez sous la forme <base_nom_image><numero_image>.<extension>.\n");
        getchar();
    }

    // Demander les informations de base
    printf("Veuillez entrer la base du nom de vos images...\n");
    char base_nom[100];
    scanf("%s", base_nom);

    printf("Veuillez entrer l'extension de vos images (sans le point)...\n");
    char extension[5];
    scanf("%s", extension);

    // Vérification de l'existence des images
    for (int i = 0; i < 6; ++i) {
        char file_image[128];
        snprintf(file_image, sizeof(file_image), "points/images/%s%d.%s", base_nom, i, extension);
        printf("Lecture %s%d.%s\n", base_nom, i, extension);
        FILE *file = fopen(file_image, "r");
        if (file == NULL) {
            perror("L'image n'a pas été trouvée");
            return 1;
        }
        fclose(file);
    }
    printf("Les images ont bien été trouvées\n");

    // Traitement si le tutoriel complet est activé
    if (caractere == 'O') {
        printf("Les 6 images vont s'afficher. Sur chacune des images, cliquez sur les 7 points apparents du cube selon l'ordre précisé sur l'image cube_tuto.jpg...\n");
    }

    // Calibration
    if (calibrage == 'N') {
        printf("Voulez-vous effectuer le calibrage? (O/N) ...\n");
        scanf(" %c", &calibrage);
    }

    char command[500];
    char couleurs_p1[4]="";
    char couleurs_p2[4]="";
    char couleurs_p3[4]="";

    if (calibrage == 'O') {
        // Sauvegarde des couleurs et exécution des scripts de calibration
        snprintf(command, sizeof(command), "python3 save_points2.py %s0.%s %s1.%s 1", base_nom, extension, base_nom, extension);
        system(command);
        while (!couleur_valide(couleurs_p1)){
            printf("Indiquer la couleur des faces latérales (BO : bleu-orange, OV : orange-vert, VR: vert-rouge, RB: rouge-bleu)\n");
            scanf("%s", couleurs_p1);
        }
        snprintf(command, sizeof(command), "python3 save_points2.py %s2.%s %s3.%s 1", base_nom, extension, base_nom, extension);
        system(command);
        while (!couleur_valide(couleurs_p2)){
            printf("Indiquer la couleur des faces latérales (BO : bleu-orange, OV : orange-vert, VR: vert-rouge, RB: rouge-bleu)\n");
            scanf("%s", couleurs_p2);
        }
        snprintf(command, sizeof(command), "python3 save_points2.py %s4.%s %s5.%s 1", base_nom, extension, base_nom, extension);
        system(command);
        while (!couleur_valide(couleurs_p3)){
            printf("Indiquer la couleur des faces latérales (BO : bleu-orange, OV : orange-vert, VR: vert-rouge, RB: rouge-bleu)\n");
            scanf("%s", couleurs_p3);
        }
        print_couleur(couleurs_p1, couleurs_p2, couleurs_p3, base_nom);

    } else {
        read_couleur(couleurs_p1, couleurs_p2, couleurs_p3, base_nom);
    }
    // Compilation et exécution des programmes liés au calibrage
    system("gcc matrice.c manipulation_fichiers.c SVD.c camera_calibration9.c -o e -lm");
    for (int i = 0; i < 6; ++i) {
        snprintf(command, sizeof(command), "./e %s%d %s", base_nom, i, i < 2 ? couleurs_p1 : (i < 4 ? couleurs_p2 : couleurs_p3));
        system(command);
    }
    // Sélection des points
    if (selection_point == 'N') {
        printf("Voulez-vous effectuer la sélection des points? (O/N) ...\n");
        scanf(" %c", &selection_point);
    }

    if (selection_point == 'O') {
        printf("Les images vont désormais être affichées par 2. Veuillez cliquer sur des points de correspondance entre les deux images dans le même ordre...\n");

        // Exécution des scripts de sélection de points
        for (int i = 0; i < 6; i += 2) {
            snprintf(command, sizeof(command), "python3 save_points.py %s%d.%s %s%d.%s 0", base_nom, i, extension, base_nom, i + 1, extension);
            system(command);
        }
    }
    // Compilation et exécution des programmes de reconstruction
    system("gcc matrice.c manipulation_fichiers.c SVD.c reconstruction.c -o r -lm");
    for (int i = 0; i < 6; i += 2) {
        snprintf(command, sizeof(command), "./r %s%d %s%d %s", base_nom, i, base_nom, i + 1, i < 2 ? couleurs_p1 : (i < 4 ? couleurs_p2 : couleurs_p3));
        system(command);
    }
    // Affichage des points par fichier et génération des résultats finaux
    snprintf(command, sizeof(command), "python3 rassemble_point.py %s0 %s2 %s4", base_nom, base_nom, base_nom);
    system(command);
    snprintf(command, sizeof(command), "python3 plot_points.py points_reconstruits-%s0 points_reconstruits-%s2 points_reconstruits-%s4", base_nom,base_nom,base_nom);
    snprintf(command, sizeof(command), "gcc triangle.c -o t -lm");
    system(command);
    snprintf(command, sizeof(command), "./t %s0", base_nom);
    system(command);
    snprintf(command, sizeof(command), "python3 plt_triangles.py %s", base_nom);
    system(command);
    return 0;
}
