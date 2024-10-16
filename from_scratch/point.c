#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include "point.h"


Point point_vide(){
    Point p;
    p.reel=matrice_nulle(3,1);          
    p.proj=matrice_nulle(3,1);          
    p.cam1=matrice_nulle(2,1);         
    p.cam2=matrice_nulle(2,1);
    return p;
}
Point* point_vide_pointeur(){
    Point* p=malloc(sizeof(Point));
    p->reel=matrice_nulle(3,1);          
    p->proj=matrice_nulle(3,1);          
    p->cam1=matrice_nulle(2,1);         
    p->cam2=matrice_nulle(2,1);
    return p;
}

matrice vect3_of_vect2(matrice v){
    matrice u=matrice_nulle(3,1);
    u.mat[0][0]=v.mat[0][0];
    u.mat[1][0]=v.mat[1][0];
    u.mat[2][0]=1;
    return u;
}

void free_point(Point* p) {
    if (p != NULL) {
        free_matrice(p->reel);  
        free_matrice(p->proj);  
        free_matrice(p->cam1);  
        free_matrice(p->cam2);  
        free(p);  
    }
}
void print_point(Point* p) {
    if (p == NULL) {
        printf("Point vide.\n");
        return;
    }

    printf("Coordonnées réelles (reel):\n");
    print_matrice(p->reel);

    printf("Coordonnées projectives (proj):\n");
    print_matrice(p->proj);

    printf("Coordonnées caméra 1 (cam1):\n");
    print_matrice(p->cam1);  

    printf("Coordonnées caméra 2 (cam2):\n");
    print_matrice(p->cam2); 
}