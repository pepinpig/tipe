#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define SEUIL 1e-3


double scalaire(double* v1, double* v2) ;
double norme(double* v) ;
double* add(double* v1, double* v2) ;
void addin(double* v1, double* v2) ;
double* sub(double* v1, double* v2) ;
void scale(double* v1, double s) ;
double* barycentre(int* l, int k, double** points) ;
double* prod(double* v1, double* v2) ;
bool nulv(double* v) ;
int trois_parmi(int n) ;
int** triangles(int card) ;
void file_print_vect(double* v, FILE* file) ;
void print_vect(double* v) ;
void destroy_trigs(int** l, int n) ;
void destroy_points(double** l, int n) ;
bool* keeptrig(int** l, int ntrig, int size, double** point) ;
double** read_points(char* filename, int count) ;
