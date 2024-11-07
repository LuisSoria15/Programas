#include<stdio.h>
#include<stdlib.h>
#include<stralign.h>
#define N 30

typedef struct nod1
{
    int clave;
    char nombre[N];
    int gen;
    struct nod1 *sig;
}TNodo1;

typedef struct nodP
{
    char carrera[N];
    struct nodP *sig;
    TNodo1 *aba;
}TNodoP;

TNodo1 *crea_nodo1(int clave, int gen, char nom[N]);
TNodoP *crea_nodoP(char carrera[N], TNodo1 *aba);

int main()
{
}

TNodo1 *crea_nodo1(int clave, int gen, char nom[N])
{
    TNodo1 *aux;
    aux = (TNodo1*)malloc(sizeof(TNodo1));

    if(aux)
    {
        aux->clave=clave;
        aux->gen=gen;
        strcpy(aux->nombre, nom);
        aux->sig=NULL;
    }
    return aux;
}

TNodoP *crea_nodoP(char carrera[N], TNodo1 *aba)
{
    TNodoP *aux;
    aux = (TNodoP*)malloc(sizeof(TNodoP));

    if(aux)
    {
        strcpy(aux->carrera, carrera);
        aux->aba= aba;
        aux->sig = NULL;
    }
}