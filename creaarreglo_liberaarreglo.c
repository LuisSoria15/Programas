#include<stdio.h>
#include<stdlib.h>

void *crea(int *tam);
void regista(int tam, void *arr);
void imprime(int tam, void *arr);

int main()
{
    void *arr;
    int tam;
    crea(&tam);
    regista(tam, arr);
    imprime(tam, arr);

    free(arr);
}

void *crea(int *tam)
{
    int *arr;
    printf("Tmaño del arreglo:\n");
    scanf("%d", tam);
    arr = (int*)malloc(sizeof(int) * (*tam));

    if(arr == NULL)
    {
        printf("No se pudor eservar memoria");
        exit(1);
    }
    return(arr);
}

void regista(int tam, void *arr)
{
    int i;

    for(i=0; i<tam; i++)
    {
        printf("Ingresa el elemento %d del arreglo:", i+1);
        scanf("%d", ((int*)arr+i));
    }
}

void imprime(int tam, void *arr)
{
    int i;

    for(i = 0; i<tam; i ++)
    {
        printf("Elemento %d: %d ", i+1, *((int*)arr+i));
    }
}