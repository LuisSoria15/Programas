//Soria Flores Luis Carlos
//Tempo de realizacion 1 hora
#include<stdio.h>
#include<stdlib.h>

typedef struct nodoD
{
    int info;
    struct nodoD *sig, *ant;
}TNodoD;

typedef struct
{
    TNodoD *inicio;
    TNodoD *fin;
}TCab;

TNodoD *crea_nodo(int num);
void inserta_inicio(TCab *cabs, int info);
void inserta_finalD(TCab *cabs, int info);
void inserta(TCab *cabs);
void registra(TCab *cabs);
void imprime_lista(TCab *cabs);

int main()
{
    TCab *cabs = (TCab *) malloc(sizeof(TCab));
    cabs->inicio = NULL;
    cabs->fin = NULL;
    registra(cabs);
    inserta(cabs);
    imprime_lista(cabs);
}

TNodoD *crea_nodo(int num)
{
    TNodoD *aux;
    aux=(TNodoD*)malloc(sizeof(TNodoD));
    if(aux)
    {
        aux->info=num;
        aux->sig=NULL;
        aux->ant=NULL;
    }
    return aux;
}

void inserta_inicio(TCab *cabs, int info)
{
    TNodoD *aux;
    aux=crea_nodo(info);
    if(aux)
    {
        aux->sig=cabs->inicio;
        if(cabs->inicio)
        {
            cabs->inicio->ant=aux;
            cabs->inicio=aux;
        }
        else
        {
            cabs->inicio=cabs->fin=aux;
        }
    }
}

void inserta_finalD(TCab *cabs, int info)
{
    TNodoD *aux;
    aux=crea_nodo(info);
    if(aux)
    {
        aux->ant=cabs->fin;
        if(cabs->fin)
        {
            cabs->fin->sig=aux;
            cabs->fin=aux;
        }
        else
        {
            cabs->inicio=cabs->fin=aux;
        }
    }
}

void inserta(TCab *cabs)
{
    TNodoD *aux, *corre;
    corre=cabs->inicio;
    if(corre->info % 2 != 0)
    {
        if(corre==cabs->inicio)
        {
            inserta_inicio(cabs, -1);
            corre = corre -> sig;
        }
    }
        while(corre->sig != NULL)
        {
            if(corre->info % 2 != 0)
            {
                aux=crea_nodo(-1);
                aux->sig = corre;
                aux->ant = corre->ant;
                if (corre->ant != NULL) 
                    {
                        corre->ant->sig = aux;
                    }
                corre->ant=aux;
            }
            corre = corre->sig;
        }
        if(corre == cabs->fin && corre->info % 2 != 0)
        {
            aux=crea_nodo(-1);
            aux->sig = corre;
            aux->ant = corre->ant;
            if (corre->ant != NULL) 
                {
                    corre->ant->sig = aux;
                }
            corre->ant=aux;
        }
}

void registra(TCab *cabs)
{
    int n, info;
    printf("Ingresa la cantidad de nodos\n");
    scanf("%d", &n);
    for(int i=0; i<n; i++)
    {
        printf("Nodo %d: \n", i+1);
        scanf("%d", &info);
        inserta_finalD(cabs, info);
    }
}

void imprime_lista(TCab *cabs)
{
    TNodoD *aux=cabs->inicio;
    int cont = 0;
    while(aux != NULL)
    {
        if(aux->info == -1)
        {
            cont++;
        }
        aux=aux->sig;
    }
    aux=cabs->inicio;
    printf("LISTA:\n");
    printf("Numero de impares: %d\n", cont);
    printf("NULL");
    while(aux != NULL)
    {
        printf("<->%d", aux->info);
        aux=aux->sig;
    }
    printf("->NULL");
}