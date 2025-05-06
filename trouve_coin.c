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
    for(y=y+1;y<=z;y++){
      score->mat[i][y]++;
    }
	}
	else {
    dy=dy/((double)k-i);
    int x = i;
    double y = j;
    int u = k;
    if(k-i<0){
      x=1;
      y = l;
      u=i;
      dy=dy;
    }
    x+=1;
    y+=1*dy;
    while(x<=u){
      score->mat[x][(int)y]++;
      score->mat[x][(int)y+1]++;
      x+=1;
      y+=dy;
    }
  }
}

bool au_bord(matrice* plan, int i, int j){
  return (i<20||i>plan->n-10||j<20||j>plan->m-20);

}

int distance(int i,int j,int k , int l){
  return (i-k)*(i-k) + (j-l)*(j-l);
}

matrice* compute_score(matrice* plan, int** actif, int size){ //size est la taille d'actif
                                                          // actif est un tableau de couple d'entier correspondant au support de plan
  matrice* score = matrice_nulle(plan->n,plan->m);
  printf("size = %d \n",size);
  for(int i = 0; i < size; i++){
    for(int j = i+1; j < size; j++){
      if(distance(actif[i][0],actif[i][1],actif[j][0],actif[j][1])<40*40 && !au_bord(plan,actif[i][0],actif[i][1]) && !au_bord(plan,actif[j][0],actif[j][1])){ //On ne traite pas de points trop au bord
																			//		printf("%d",j);
        update_score(plan,score,actif[i][0],actif[i][1],actif[j][0],actif[j][1]);
      }
    }
  }
  printf("debug");
  fflush(stdout);
  return score;
}
  
void filtre_mat(matrice* input, int** actif, int size){
  matrice* tmp = matrice_nulle(input->n,input->m);
  for(int i = 0;i<size;i++){
    if(!au_bord(input,actif[i][0],actif[i][1])){
      tmp->mat[actif[i][0]][actif[i][1]]=1;
    }
  }
  for(int i = 0;i<input->n;i++){
    for(int j = 0;j<input->m;j++){
			if(tmp->mat[i][j]==1 && input->mat[i][j]<2000){
				printf("gardé\n");
				input->mat[i][j]=1;
			}
			else {
				input->mat[i][j]=0;
			}
		}
	}
	free_matrice(tmp);
  /*for(int i = 0;i<input->n;i++){
    for(int j = 0;j<input->m;j++){
      if(au_bord(input,i,j))
      input->mat[i][j]=1;
      else input->mat[i][j]=0;
    }
  }
  update_score(input,input,20,input->m-20,20,20);*/

}

void pretty_mat(matrice* input, int** actif, int size){
  matrice* tmp= matrice_nulle(input->n,input->m);
  for(int i = 0;i<size;i++){
    if(!au_bord(input,actif[i][0],actif[i][1])){
      tmp->mat[actif[i][0]][actif[i][1]]=1;
    }
  }
  for(int i = 0;i<input->n;i++){
    for(int j = 0;j<input->m;j++){
      if(tmp->mat[i][j]==1){
        printf("%f\n",input->mat[i][j]);
      }
    }
  }
  int s = 0;
  for(int i = 0;i<input->n;i++){
    for(int j = 0;j<input->m;j++){
			if(tmp->mat[i][j]==1 && input->mat[i][j]<700){
        s++;
        for(int k = -5;k<5;k++){
          for(int l= -5;l<5;l++){
            //printf("i+k = %d j+l = %d m = %d n = %d\n",i+k,j+l,input->m,input->n);
            fflush(stdout);
            input->mat[i+k][j+l]=1;
            tmp->mat[i+k][j+l]=0;
          }
        }
			}
			else {
        input->mat[i][j]=0;
			}
		}
	}
  printf("     %d   %d   ",s,size);
	free_matrice(tmp);
}

