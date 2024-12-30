#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void type_simple_triangle (int a, int b, int c){
//hyp : a>=0 && b>=0 && c>=0
	if (a==b){
		if(b==c)
			printf("équilatéral");
		else	
			printf("isocèle");
	}
	else{
		if((b==c) || (a==c))
			printf("isocèle");
		else
			printf("scalène");	
	}
	printf("\n");
}




bool est_triangle(int a, int b, int c){
	return (a<=b+c) && (b<=a+c) && (c<=a+b) ;
}







bool est_plat(int a, int b, int c){
	return (a==b+c) || (b==a+c) || (c==a+b) ;
}







bool est_rectangle(int a, int b, int c){
	return (a*a + b*b == c*c) || (b*b + c*c == a*a) || (a*a + c*c == b*b) ;
}







void affiche_type_triangle(int a,int b, int c){
	if (!est_triangle(a,b,c))
		printf("Ces longueurs ne sont les 3 côtés d'un même triangle\n");
	else{
		if (a==b){
			if(b==c){
				 if (a==0)// alors a=b=c=0
				 	printf("équilatéral plat = réduit à un point");
				 else
					printf("équilatéral");
				}
			else if(2*b*b == c*c)	// dans ce cas a²+b²=c²
				printf("isocèle rectangle");
			else if (a+b == c)
				printf("isocèle plat");
			else
				printf("isocèle");
		}
		else{ // ici on sait que a!=b
			if (b == c) {
				if(2*b*b == a*a)	// dans ce cas c²+b²=a²
					printf("isocèle rectangle");
				else if (b+c == a)
					printf("isocèle plat");
				else
					printf("isocèle");
			}		
			else if (a == c){
				// ici on sait que a!=b et b!=c et a=c
				if(2*a*a == b*b)	// dans ce cas a²+c²=b²
					printf("isocèle rectangle");
				else if (a+c == b)
					printf("isocèle plat");
				else
					printf("isocèle");		
			}
			else{ // ici on sait que a!=b et b!c et a!=c
				if (est_plat(a,b,c))
					printf("plat");
				else if (est_rectangle(a,b,c))
					printf("rectangle");
				else
					printf("scalène");
			}
		}
		printf("\n");	
	}
}


int main(){
	// jeu de test de type_triangle
	type_simple_triangle (1,1,1); //equi
	type_simple_triangle (1,1,2); //iso
	type_simple_triangle (1,2,1); //iso
	type_simple_triangle (2,1,1); //iso
	type_simple_triangle (1,2,3); //scalene
	
	
	
	// jeu de test de est_triangle
	assert(est_triangle (1,2,3));
	assert(est_triangle (3,4,5));
	assert(est_triangle (1,1,1));
	assert(!est_triangle (3,4,15));
	assert(!est_triangle (31,4,15));

	
	
	
	// jeu de test de est_plat
	assert(est_plat (1,2,3));
	assert(!est_plat (3,4,5));
	
	
	// jeu de test de est_rectangle
	assert(!est_rectangle (1,2,3));
	assert(est_rectangle (3,4,5));


	// jeu de test de affiche_type_triangle
	affiche_type_triangle (1,1,1); //equi
	affiche_type_triangle (0,0,0); // equi plat = pt
	affiche_type_triangle (1,1,2); //iso plat
	affiche_type_triangle (1,2,1); //iso plat
	affiche_type_triangle (2,1,1); //iso plat
	affiche_type_triangle (3,3,5); //iso
	affiche_type_triangle (1,2,3); //plat
	affiche_type_triangle (3,4,5); //rectangle
	affiche_type_triangle (3,4,6); //scalène
	//isocèle rectangle n'arrive pas avec des longueurs entières
	
	return 0;
}
