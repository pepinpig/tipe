#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

// produit scalaire vecteurs de dimension 3
double scalaire (double* v1, double*  v2){
	double s = 0;
	for (int i = 0;i<3;i++){
		s+= v1[i]*v2[i];
	}
	return s;
}

double* add(double* v1, double* v2){
	double* v3 = malloc(sizeof(double)*3);
	for(int i=0;i<3;i++){
		v3[i]= v1[i]+v2[i];
	}
	return v3;
}

void addin(double* v1, double* v2){
	for(int i=0;i<3;i++){
		v1[i]= v1[i]+v2[i];
	}
}

double* sub(double* v1, double* v2){
	double* v3 = malloc(sizeof(double)*3);
	for(int i=0;i<3;i++){
		v3[i]= v1[i]-v2[i];
	}
	return v3;
}

void scale(double* v1, double s){
	for(int i=0;i<3;i++){
		v1[i]= s*v1[i];
	}
}

double* barycentre(int* l, int k, double** points){
	double* bary=malloc(3*sizeof(double));
	scale(bary,0);
	for(int i=0;i<k;i++){
		addin(bary,points[l[i]]);
	}
	scale(bary,(double)1/k);
	return bary;
}

// produit vectoriel vecteurs de dimension 3
double* prod (double* v1, double* v2){
	double* v3 = malloc(sizeof(double)*3) ;
	for(int i=0;i<3;i++){
		v3[i]= v1[(i+1)%3]*v2[(i+2)%3]
					-v1[(i+2)%3]*v2[(i+1)%3];
	}
	return v3;
}

bool nulv (double* v){
	int i =0;
	while(i<3){
		if(v[i]!=0){
			return false;
		}
		i++;
	}
	return true;
}

int trois_parmi(int n){
	return (n<=2)?0:n*(n-1)*(n-2)/6;
}

int** triangles(int card){
	int n = trois_parmi(card);
	int** trigs= malloc(sizeof(int*)*n);
	for(int i=0;i<n;i++){
		trigs[i]=malloc(sizeof(int)*3);
	}
	int ind = 0;
	for(int i = 0;i < card ; i++){
		for(int j = i+1;j < card ; j++){
		 	for(int k = j+1; k < card ; k++){
				trigs[ind][0]=i;
				trigs[ind][1]=j;
				trigs[ind][2]=k;
				ind++;
			}
		}
	}
	return trigs;
}

void print_vect (double* v){
	printf("(%1.3f,%1.3f,%1.3f)",v[0],v[1],v[2]);
}
void print_int_vect (int* v){
	printf("(%d,%d,%d)",v[0],v[1],v[2]);
}

void destroy_trigs (int** l, int n){
	for(int i = 0; i<n; i++)
		free(l[i]);
	free(l);
}
void destroy_points (double** l, int n){
	for(int i = 0; i<n; i++)
		free(l[i]);
	free(l);
}

bool* keeptrig (int**l, int ntrig, int size, double** point){
	bool* res = malloc(ntrig*sizeof(bool));
	for(int i = 0;i<ntrig;i++){
		double* v1= sub(point[l[i][1]],point[l[i][0]]);
		double* v2= sub(point[l[i][2]],point[l[i][0]]);
		double* n = prod(v1,v2);
		if (nulv(n)){
			res[i]=false;
		}
		else {
			double* bary = barycentre(l[i],3,point);
			res[i]=true;
			int signe = 0;
			for(int j=0;j<size;j++){
				double* v= sub(bary,point[j]);
				/*printf("\n");
				print_vect(v);
				printf("\n");
				print_vect(n);
				printf("\n");*/
				double s = scalaire(n,v);
				free (v);
				//printf("\n scalaire %d = %lf \n",j,s);
				if (s!=0){
					if (signe == 0){
						signe = (s>0)?1:(-1);
					}
					else if (signe*s< 0){
						res[i]=false;
						break;
					}
				}
				//printf("\n signe  = %d\n",signe);
			}
			free(bary);
		}
		free(v1);
		free(v2);
		free(n);
	}
	return res;
}



int main(){
	srand(clock());
	double **env = malloc(sizeof(double*)*20);
	for(int i = 0;i < 20 ; i ++){
		env[i] = malloc(sizeof(double)*3);
		env[i][0] = (double)(rand()%1000)/1000;
		env[i][1] = (double)(rand()%1000)/1000;
		env[i][2] = (double)(rand()%1000)/1000;
	}
/*	env[0][0]=0;
	env[0][1]=0;
	env[0][2]=0;
	env[1][0]=1;
	env[1][1]=0;
	env[1][2]=0;
	env[2][0]=0;
	env[2][1]=1;
	env[2][2]=0;
	env[3][0]=0;
	env[3][1]=0;
	env[3][2]=1;
	env[4][0]=0;
	env[4][1]=1;
	env[4][2]=1;
	env[5][0]=1;
	env[5][1]=0;
	env[5][2]=1;
	env[6][0]=1;
	env[6][1]=1;
	env[6][2]=0;
	env[7][0]=1;
	env[7][1]=1;
	env[7][2]=1;*/
	int n = trois_parmi(20);
	int** test = triangles(20);
	for(int i = 0;i<n;i++){
		print_int_vect(test[i]);
		printf(",");
	}
	printf("\n");
	for(int i = 0;i<20;i++){
		print_vect(env[i]);
		printf(",");
	}
	int s = 0;
	printf("\n");
	bool * garde = keeptrig(test,n,20,env);
	for(int i = 0;i<n;i++){
		printf("triangle %d : %d\n",i,(garde[i])?1:0);
		if(garde[i])
			s++;
	}
	printf("s=%d",s);
	printf("\n");
	free(garde);
	
	destroy_trigs(test,n);
	destroy_points(env,20);
}
