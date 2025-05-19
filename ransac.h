#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "constante.h"
#include "matrice.h"
#include "manipulation_fichier.h"


double point_to_line_dist(double x, double y, double a, double b, double c);
void detect_lines_and_extremities(matrice* points);