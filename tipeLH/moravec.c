#include "moravec.h"

// Calcul de la variance pour une direction donnée
double variance(matrice* image, int x, int y, int dx, int dy) {
    int count = 0; int sum = 0; int sumSq = 0;
    for (int i = -WINDOW; i <= WINDOW; ++i) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < image->m && ny >= 0 && ny < image->n) {
            int val = image->mat[ny][nx];
            sum += val;
            sumSq += val * val;
            count++;
        }
    }
    return (count > 0) ? (sumSq - (sum * sum) / count) : 0;
}

// Application de l'algorithme de Moravec pour la détection des coins
int moravec(matrice* image_input, matrice* image_output) {
    int count=0;
    for (int y = 1; y < image_input->n - 1; y++) {
        for (int x = 1; x < image_input->m - 1; x++) {
            int var0 = variance(image_input, x, y, 0, 1);  // Verticale
            int var1 = variance(image_input, x, y, 1, 0);  // Horizontale
            int var2 = variance(image_input, x, y, 1, 1);  // Diagonale bas gauche haut droite
            int var3 = variance(image_input, x, y, 1, -1); // Diagonale haut gauche bas droite

            int vartot = var0;  // Commence par la première direction
            if (PARAM){
                if (var1 < vartot) vartot = var1;
                if (var2 < vartot) vartot = var2;
                if (var3 < vartot) vartot = var3;
            }
            else{
                vartot += var1;
                vartot += var2;
                vartot += var3;
                vartot /= 4;
            }
            if (vartot > SEUIL) {
                image_output->mat[y][x] = 1;  // Coin détecté
                count++;
            } else {
                image_output->mat[y][x] = 0;  // Pas de coin
            }
        }
    }
    return count;
}

int moravec_arr(matrice* image_input, int*** detected){
  matrice* tmp = matrice_nulle_pointeur(image_input->n,image_input->m);
  int size = moravec(image_input,tmp);
  *detected= malloc(sizeof(int*)*size);
  for(int i = 0;i<size;i++){
    (*detected)[i]=malloc(sizeof(int)*2);
  }
  int k = 0;
  for(int i = 0; i < tmp->n; i++){
    for(int j = 0; j < tmp->m; j++){
      if(tmp->mat[i][j]==1){
        (*detected)[k][0]=i;
        (*detected)[k][1]=j;
        k++;
      }
    }
  }
  return size;
}

