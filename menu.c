#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "constante.h"

#define MAX_PATH 256

char image_name[MAX_PATH] = "";
int selected_option = 0;

void run_command(const char *command) {
    endwin();
    system(command);
    printf("\nAppuyez sur Entree pour revenir au menu...");
    getchar();
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
}

void afficher_menu_options(const char *titre, const char **options, int n, int highlight, int debut_ligne) {
    clear();
    mvprintw(0, 2, "%s", titre);
    for (int i = 0; i < n; i++) {
        if (i == highlight) attron(A_REVERSE);
        mvprintw(i + debut_ligne, 4, "%s", options[i]);
        attroff(A_REVERSE);
    }
    mvprintw(debut_ligne + n + 1, 2, "Utilisez les fleches pour naviguer, Entree pour valider.");
    refresh();
}


void executer_detection(const char *mode) {
    char cmd[3000];
    for (int i = 0; i < 7; i += 2) {
        snprintf(cmd, sizeof(cmd), "./test_detection %s%d %s%d %s", image_name, i, image_name, i+1, mode);
        endwin();
        system(cmd);
    }
    printf("\nAppuyez sur Entree pour revenir au menu...");
    getchar();
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
}

void executer_calibration(const char *mode) {
    run_command("make clean && make test_camera_calibration");
    char cmd[3000];
    for (int i = 0; i < 8; i++) {
        snprintf(cmd, sizeof(cmd), "./test_camera_calibration %s%d VR %s", image_name, i, mode);
        run_command(cmd);
    }
}

void executer_reconstruction() {
    char cmd[3000];
    snprintf(cmd, sizeof(cmd), "./test_reconstruction_mult %s0 %s1 %s2 %s3 %s4 %s5 %s6 %s7",
             image_name, image_name, image_name, image_name,
             image_name, image_name, image_name, image_name);
    run_command(cmd);
}

void executer_triangulation() {
    char cmd[3000];
    snprintf(cmd, sizeof(cmd), "./test_triangulation %s0 %s1 %s2 %s3 %s4 %s5 %s6 %s7",
             image_name, image_name, image_name, image_name,
             image_name, image_name, image_name, image_name);
    run_command(cmd);
}

int main(int argc,char* argv[]) {
    printf("compilation...");
    char cmd_main[200];
    snprintf(cmd_main, sizeof(cmd_main),"make all");
    system(cmd_main);
    const char *menu_principal[] = { "Detection", "Calibration", "Reconstruction", "Triangulation", "Quitter" };
    const int nb_principal = sizeof(menu_principal) / sizeof(menu_principal[0]);

    const char *menu_detection[] = { "Mode Moravec", "Mode Select", "Mode Rien", "Retour" };
    const int nb_detection = sizeof(menu_detection) / sizeof(menu_detection[0]);

    const char *menu_calibration[] = { "Mode Select", "Mode Rien", "Retour" };
    const int nb_calibration = sizeof(menu_calibration) / sizeof(menu_calibration[0]);

    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);
    echo();
    if (argc < 2) {
        echo();
        mvprintw(2, 2, "Entrez le nom de base de l'image (ex: dodecf): ");
        getnstr(image_name, MAX_PATH - 1);
        noecho();
    } else {
        strncpy(image_name, argv[1], MAX_PATH - 1);
        image_name[MAX_PATH - 1] = '\0';
    }    
    int highlight = 0, ch;
    while (1) {
        afficher_menu_options("Menu Principal", menu_principal, nb_principal, highlight, 2);
        ch = getch();
        if (ch == KEY_UP && highlight > 0) highlight--;
        else if (ch == KEY_DOWN && highlight < nb_principal - 1) highlight++;
        else if (ch == '\n') {
            if (highlight == 0) {
                int sub = 0;
                while (1) {
                    clear();
                    mvprintw(0, 2, "Menu Detection");
                    mvprintw(2, 2, "Constantes actuelles:");
                    mvprintw(3, 4, "Distance_seuil = %d", Distance_seuil);
                    mvprintw(4, 4, "Hamming_seuil = %d", Hamming_seuil);
                    mvprintw(5, 4, "Window = %d", Window);
                    mvprintw(6, 4, "Seuil_moravec = %d", Seuil_moravec);
                    mvprintw(7, 4, "Seuil_tc = %d", Seuil_tc);
                    mvprintw(8, 4, "Dist_tc = %d", Dist_tc);
                    for (int i = 0; i < nb_detection; i++) {
                        if (i == sub) attron(A_REVERSE);
                        mvprintw(10 + i, 4, "%s", menu_detection[i]);
                        attroff(A_REVERSE);
                    }
                    mvprintw(10 + nb_detection + 1, 2, "Utilisez les fleches pour naviguer, Entree pour valider.");
                    refresh();
                    ch = getch();
                    if (ch == KEY_UP && sub > 0) sub--;
                    else if (ch == KEY_DOWN && sub < nb_detection - 1) sub++;
                    else if (ch == '\n') {
                        if (sub == 0) executer_detection("moravec");
                        else if (sub == 1) executer_detection("select");
                        else if (sub == 2) executer_detection("");
                        else break;
                    }
                }
            } else if (highlight == 1) {
                int sub = 0;
                while (1) {
                    afficher_menu_options("Menu Calibration", menu_calibration, nb_calibration, sub, 2);
                    ch = getch();
                    if (ch == KEY_UP && sub > 0) sub--;
                    else if (ch == KEY_DOWN && sub < nb_calibration - 1) sub++;
                    else if (ch == '\n') {
                        if (sub == 0) executer_calibration("select");
                        else if (sub == 1) executer_calibration("");
                        else break;
                    }
                }
            } else if (highlight == 2) {
                executer_reconstruction();
            } else if (highlight == 3) {
                executer_triangulation();
            } else if (highlight == 4) {
                break;
            }
        }
    }

    endwin();
    snprintf(cmd_main, sizeof(cmd_main),"make clean");
    system(cmd_main);
    return 0;
}
