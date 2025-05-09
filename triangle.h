#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "matrice.h"
#include <stdint.h>

struct triangle_s{
  int16_t a;
  int16_t b;
  int16_t c;
};
typedef struct triangle_s triangle;

double scalaire(double* v1, double* v2) ;
double norme(double* v) ;
void add(double* v1, double* v2, double* v3) ;
void addin(double* v1, double* v2) ;
void sub(double* v1, double* v2, double* v3) ;
void scale(double* v1, double s) ;
void barycentre(triangle l, double* bary, double** points) ;
void prod(double* v1, double* v2, double* v3) ;
bool nulv(double* v) ;
unsigned long int trois_parmi(int n) ;
triangle* triangles(int card) ;
void file_print_vect(double* v, FILE* file) ;
void print_vect(double* v) ;
void destroy_trigs(triangle* l);
void destroy_points(double** l, int n) ;
bool* keeptrig(triangle* l,unsigned long int ntrig, int size, double** point) ;
double** read_points(char* filename, int count) ;
void stl_generate(char* filename, double** point, triangle* l,unsigned long int ntrig,bool *garde);
double ** rand_points(int n);
double** mat_to_table (matrice* mat,int* n);
