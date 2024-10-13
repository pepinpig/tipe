#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "matrice.h"
#include "point.h"


Point point_vide(){
    Point p;
    matrice reel=matrice_nulle(3,1);          
    matrice proj=matrice_nulle(3,1);          
    matrice cam1=matrice_nulle(2,1);         
    matrice cam2=matrice_nulle(2,1);
    return p;
}
Point* point_vide_pointeur(){
    Point* p=malloc(sizeof(Point));
    matrice reel=matrice_nulle(3,1);          
    matrice proj=matrice_nulle(3,1);          
    matrice cam1=matrice_nulle(2,1);         
    matrice cam2=matrice_nulle(2,1);
    return p;
}

matrice vect3_of_vect2(matrice v){
    matrice u=matrice_nulle(3,1);
    u.mat[0][0]=v.mat[0][0];
    u.mat[1][0]=v.mat[1][0];
    u.mat[2][0]=1;
    return u;
}



