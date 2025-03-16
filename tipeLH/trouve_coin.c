#include "trouve_coin.h"

void update_score(matrice* plan, matrice* score, int i, int j,int k, int l){
	double dy = l - j;
	if (k-i==0){
    int y=j;
    int z=l;
    if(j>l){
      y=l;
      z=j;
    }
    for(;y<=z;y+=100){
      if(plan->mat[i][y]==1){
        score->mat[i][y]++;
      }
    }
	}
	else {
    dy=dy/((double)k-i);
    int x = i;
    double y = j;
    int u = k;
    if(k-i<0){
      x=k;
      y = l;
      u=i;
      dy=-dy;
    }
    while(x<=u){
        score->mat[x][(int)y]++;
        score->mat[x][(int)y+1]++;
      x+=100;
      y=+dy;
    }
  }
}

matrice* compute_score(matrice* plan, int** actif, int size){ //size est la taille d'actif
                                                          // actif est un tableau de couple d'entier correspondant au support de plan
  matrice* score = matrice_nulle_pointeur(plan->n,plan->m);
  for(int i = 0; i < size; i++){
      printf("i = %d\n",i);
    for(int j = i+1; j < size; j++){
      if((actif[i][1]< plan->m - 1 && actif[j][1] < plan->m -1)&&(actif[i][1] > 1 && actif[j][1] > 1)){ //On ne traite pas de points trop au bord
        update_score(plan,score,actif[i][0],actif[i][1],actif[j][0],actif[j][1]);
      }
    }
  }
  return score;
}
  
void filtre_mat(matrice* input, int** actif, int size){
  matrice tmp= matrice_nulle(input->n,input->m);
  for(int i = 0;i<size;i++){
    tmp.mat[actif[i][0]][actif[i][1]]=1;
  }
  for(int i = 0;i<input->n;i++){
    for(int j = 0;j<input->m;i++){
      input->mat[i][j]*=tmp.mat[i][j];
    }
  }
  free_matrice(tmp);
}

