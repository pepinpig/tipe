#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 256

char image1_name[MAX_PATH] = "";
char image2_name[MAX_PATH] = "";
int selected_image = 1;  // 1 pour image1, 2 pour image2

void run_command(const char *binary, const char *image) {
    char command[512];
    snprintf(command, sizeof(command), "%s %s", binary, image);
    endwin();  // Quit ncurses before executing command
    system(command);
    printf("\nAppuyez sur Entrée pour revenir au menu...");
    getchar(); getchar();  // Pause
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);  // Restart ncurses
}

void run_command2(const char *binary, const char *image1, const char *image2, const char *option) {
    char command[512];
    snprintf(command, sizeof(command), "%s %s %s %s", binary, image1, image2, option);
    endwin();  // Quit ncurses before executing command
    system(command);
    printf("\nAppuyez sur Entrée pour revenir au menu...");
    getchar(); getchar();  // Pause
    initscr(); cbreak(); noecho(); keypad(stdscr, TRUE);  // Restart ncurses
}

void afficher_menu(int highlight) {
    const char *options[] = {
        "Recherche de points d'interets",
        "Appariement",
        "Calibration",
        "Nuage de points",
        "Triangulation",
        "Quitter"
    };

    clear();
    mvprintw(0, 2, "Pipeline de reconstruction 3D");
    mvprintw(1, 2, "Image 1 chargee : %s", image1_name[0] ? image1_name : "Aucune");
    mvprintw(2, 2, "Image 2 chargee : %s", image2_name[0] ? image2_name : "Aucune");
    mvprintw(3, 2, "Image selectionnee pour recherche de points : %s", (selected_image == 1 ? image1_name : image2_name));

    for (int i = 0; i < 6; i++) {
        if (i == highlight)
            attron(A_REVERSE);
        mvprintw(i + 5, 4, "%s", options[i]);  // Correction ici : on commence à la ligne 5 pour laisser de l'espace
        attroff(A_REVERSE);
    }
    mvprintw(12, 2, "Utilisez les fleches pour naviguer, Entree pour valider.");
    refresh();
}


void afficher_menu_appariement(int highlight) {
    const char *options[] = {
        "Selection manuelle des points",
        "Moravec (points d'interets)",
        "Sans selection (utiliser points pre-sélectionnés)",
        "Retour"
    };

    clear();
    mvprintw(0, 2, "Choisissez une méthode pour l'appariemment");
    for (int i = 0; i < 4; i++) {
        if (i == highlight)
            attron(A_REVERSE);
        mvprintw(i + 2, 4, "%s", options[i]);
        attroff(A_REVERSE);
    }

    mvprintw(7, 2, "Utilisez les fleches pour naviguer, Entree pour valider.");
    mvprintw(8, 2, "Description: %s", (highlight == 0) ? "Selection manuelle des points." :
                             (highlight == 1) ? "Utilisation de l'algorithme de Moravec pour trouver des points d'interets." :
                             (highlight == 2) ? "Utilise des points déjà sélectionnés précédemment." : "Retour au menu principal.");
    refresh();
}

void demander_nom_images() {
    echo();
    clear();
    mvprintw(2, 2, "Entrez le nom de la premiere image (ex: image1.pgm) : ");
    getnstr(image1_name, MAX_PATH - 1);

    mvprintw(4, 2, "Entrez le nom de la deuxieme image (ex: image2.pgm) : ");
    getnstr(image2_name, MAX_PATH - 1);

    noecho();
}

void choisir_image_pour_points() {
    int ch;
    while (1) {
        clear();
        mvprintw(0, 2, "Choisissez l'image pour la recherche de points d'interets");
        mvprintw(1, 2, "1: %s", image1_name);
        mvprintw(2, 2, "2: %s", image2_name);
        mvprintw(4, 2, "Utilisez les fleches pour naviguer et Entree pour valider.");
        refresh();

        ch = getch();
        if (ch == KEY_UP || ch == KEY_DOWN) {
            selected_image = (selected_image == 1) ? 2 : 1;
        } else if (ch == '\n') {
            break;
        }
    }
}

int main() {
    int ch;
    int highlight = 0;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    demander_nom_images();

    while (1) {
        afficher_menu(highlight);
        ch = getch();

        if (ch == KEY_UP && highlight > 0)
            highlight--;
        else if (ch == KEY_DOWN && highlight < 4)
            highlight++;
        else if (ch == '\n') {
            switch (highlight) {
                case 0: 
                    choisir_image_pour_points();
                    run_command("./build/test_moravec", (selected_image == 1 ? image1_name : image2_name));
                    break;
                case 1: {
                    int subhighlight = 0;
                    while (1) {
                        afficher_menu_appariement(subhighlight);
                        ch = getch();

                        if (ch == KEY_UP && subhighlight > 0)
                            subhighlight--;
                        else if (ch == KEY_DOWN && subhighlight < 3)
                            subhighlight++;
                        else if (ch == '\n') {
                            if (subhighlight == 3) break;  // Retour au menu principal
                            const char *option = (subhighlight == 0) ? "select" : (subhighlight == 1) ? "moravec" : "";
                            run_command2("./build/test_detection", image1_name, image2_name, option);
                            break;
                        }
                    }
                    break;
                }
                case 2: run_command2("./build/test_nuage", image1_name, image2_name, ""); break;
                case 3: run_command2("./build/test_nuage", image1_name, image2_name, ""); break;
                case 4: run_command2("./build/test_triangulation", image1_name, image2_name, ""); break;
                case 5: endwin(); return 0;
            }
        }
    }

    endwin();
    return 0;
}
