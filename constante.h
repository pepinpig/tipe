#ifndef CONSTANTES_H
#define CONSTANTES_H

extern int Distance_seuil;
extern int Hamming_seuil;
extern int Window;
extern int Seuil_moravec;
extern int Seuil_tc;
extern int Dist_tc;
extern int Seuil_triangle;

extern int RANSAC_ITER ; //	Nombre de tentatives aléatoires 
extern int DIST_THRESHOLD ; //Distance max pour être considéré comme aligné
extern int MIN_INLIERS; // Nombre minimal de points alignés pour valider une droite
#endif // CONSTANTES_H
