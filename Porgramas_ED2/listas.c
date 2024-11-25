#include <stdio.h>
#include <stdlib.h>

typedef struct nodo1
{
    int info;
    struct nodo1 *sig;
} TNodo;

TNodo *crea_nodo(int dato);
void inserta_inicio(TNodo **cab, int dato);
void imprime_nodo(TNodo *cab, int dato);
void inserta_final(TNodo **cab, int dato);
void inserta_finalRec(TNodo **cab, int dato);
void inserta_ordenado(TNodo **cab, int dato);
void inserta_odrenadoRec(TNodo **cab, int dato);
int elimina_nodo_inicio(TNodo **cab, int *dato);
void menu(TNodo *cab);


int main()
{
}

void imprime_nodo(TNodo *cab, int dato)
{
    TNodo *aux;
    aux->info = dato;
    if (aux != NULL)
    {
        printf("dato: %d\n", aux->info);
    }
}

TNodo *crea_nodo(int dato)
{
    TNodo *aux;
    aux = (TNodo *)malloc(sizeof(TNodo));
    if (aux)
    {
        aux->info = dato;
        aux->sig = NULL;
    }
    return aux;
}

void inserta_inicio(TNodo **cab, int dato)
{
    TNodo *aux;

    aux = crea_nodo(dato);
    if (aux != NULL)
    {
        aux->sig = *cab;
        *cab = aux;
    }
}

void inserta_final(TNodo **cab, int dato)
{
    TNodo *aux, *corre;
    aux = crea_nodo(dato);
    if (aux)
    {
        *cab = NULL;
        if (*cab == NULL)
        {
            *cab = aux;
        }
        else
        {
            corre = *cab;
            while (corre->sig != NULL)
            {
                corre = corre->sig;
            }
            corre->sig = aux;
        }
    }
}

void inserta_finalRec(TNodo **cab, int dato)
{
    if(*cab==NULL)
    {
        *cab = crea_nodo;
    }
    else
    {
        inserta_finalRec(&(*cab)->sig, dato);
    }
}

void inserta_ordenado(TNodo **cab, int dato)
{
    TNodo *aux, *corre, *ant;

    if(*cab == NULL || (*cab)->info > dato)
    {
        aux=crea_nodo(dato);
        aux->sig=*cab;
        *cab=aux;
    }
    else
    {
        corre=*cab;
        while(corre != NULL && corre->info < dato)
        {
            ant=corre;
            corre=corre->sig;
        }
        aux=crea_nodo(dato);
        aux->sig=corre;
        ant->sig=aux;
    }
}

void inserta_odrenadoRec(TNodo **cab, int dato)
{
    TNodo *aux, *corre, *ant;

    if(*cab==NULL || (*cab)->info > dato)
    {
        aux=crea_nodo(dato);
        aux->sig=*cab;
        *cab=aux;
    }
    else
    {
        inserta_odrenadoRec(&(*cab)->sig, dato);
    }
}

int elimina_nodo_inicio(TNodo **cab, int *dato)
{
    TNodo *aux;
    int ban=0;
    if(*cab != NULL)
    {
        aux=*cab;
        *cab=(*cab)->sig;
        ban = aux->info;
        free(aux);
    }
    return ban;
}