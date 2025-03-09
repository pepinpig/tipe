#pragma once

matrice* contruction_A(long double* X, long double* Y, long double* Z, long double* u, long double* v);
void camera_calibration_resolution(matrice* P, matrice* A);
void camera_calibration_main(char* image_name, char* cl);