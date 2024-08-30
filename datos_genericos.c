#include<stdio.h>
#include<stdlib.h>
#include <malloc.h>
#define N 1

void *crea_dato_generico(int *tipo);
void captura_dato_generico(void *ptr, int tipo);
void imprime_dato_generico(void*ptr, int tipo);

int main()
{
    int tipo;
    void *ptr;
    ptr = crea_dato_generico(&tipo);
    captura_dato_generico(ptr, tipo);
    imprime_dato_generico(ptr, tipo);
}

void *crea_dato_generico(int *tipo)
{
    void *ptr;
    printf("Tipo(1 int 2 float 3 char 4 cadena 5 alumno)");
    scanf("%d", tipo);
    switch (*tipo)
    {
    case 1:ptr=malloc(sizeof(int));
        break;
    case 2:ptr=malloc(sizeof(float));
        break;
    }
    return ptr;
}
void captura_dato_generico(void *ptr, int tipo)
{
    switch(tipo)
    {
    case 1: printf("entero: ");
            scanf("%d", (int*)ptr);
        break;
    case 2: printf("flotante");
            scanf("%f", (float*)ptr);
    }
}

void imprime_dato_generico(void *ptr, int tipo)
{
    switch(tipo)
    {
    case 1: printf("entero: %d", (int*)ptr);
        break;
    case 2: printf("Flotante: %f", (float*)ptr);
        break;
    }
}