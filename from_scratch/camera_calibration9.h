#pragma once

void load_all_points_images(const char* filename, float* u, float*v);
void load_all_points_reels(const char* filename, float* X, float* Y, float* Z);
matrice* contruction_A(float* X, float* Y, float* Z, float* u, float* v);
void camera_calibration_resolution(matrice* P, matrice* A);