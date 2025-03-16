#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct lst1_cellule{
    int k;
    struct lst1_cellule* next;
} lst1_cellule;

typedef lst1_cellule* lst1;

typedef struct lst2_c{
    int k;
    struct lst2_c* prev;
    struct lst2_c* next;
} lst2_c;

typedef lst2_c* lst2;

typedef struct lst3_c{
    int k;
    char* s;
    struct lst3_c* next;
} lst3_c;

typedef lst3_c* lst3;

typedef struct lst4_c{
    int k;
    int* tab;
    struct lst4_c* next;
} lst4_c;

typedef lst4_c* lst4;

lst1 creer_liste(int k){
    lst1 res=malloc(sizeof(lst1_cellule));
    res->k=k;
    res->next=NULL;
    return res;
}
//test
void print_liste(lst1 l){
    lst1 act=l;
    printf("[");
    while(act!=NULL){
        printf("%d",act->k);
        if (act->next!=NULL){
            printf(", ");
        }
        act=act->next;
    }
    printf("]\n");
}

lst1 init_n_liste(int n){
    lst1 res=creer_liste(0);
    lst1 act=res;
    for (int i = 1; i < n; i++){
        lst1 supp=creer_liste(i);
        act->next=supp;
        act=supp;
    }    
    return res;
}

void free_lst1(lst1 l){
    lst1 act;
    while (l!=NULL){
        act=l->next;
        free(l);
        l=act;
    }
}


int main(){
    lst1 l=init_n_liste(5);
    print_liste(l);
    free_lst1(l);
    return 0;
}
