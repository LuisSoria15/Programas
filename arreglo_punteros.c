#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct
{
    int clave;
    char nombre[30];
}TAlumno;

void *crea_espacio(int *tipo, int *tam);
void captura_arreglo(int tipo, void *arr, int tam);
void busca_elemento(int tipo, void *arr, int tam);
void imprime_arreglo(void *arr, int tipo, int tam);

int main()
{
    TAlumno TAlumno;
    void *arr;
    int tam;
    int tipo;
    crea_espacio(&tipo, &tam);
    captura_arreglo(tipo, arr, tam);
    busca_elemento(tipo, arr, tam);
    imprime_arreglo(arr, tipo, tam);
}

void *crea_espacio(int *tipo, int *tam)
{
    int *arr;
    printf("Dame el tipo:\n 1 int\n 2 float: \n 3 char:\n 4 clave:\n");
    scanf("%d", tipo);
    printf("Tamaño del arreglo:\n");
    scanf("%d", tam);
    switch(*tipo)
    {
        case 1: arr = malloc(sizeof(int) * (*tam));
        break;
        case 2: arr = malloc(sizeof(float) * (*tam));
        break;
        case 3: arr = malloc(sizeof(char) * (*tam));
        break;
        case 4: arr = malloc(sizeof(TAlumno) * (*tam));
    }
    return arr;
}

void captura_arreglo(int tipo, void *arr, int tam)
{
    int i;
    switch(tipo)
    {
        case 1:
        for(i=0; i<tam; i++)
        {
            printf("Elemento %d: \n", i+1);
            scanf("%d", ((int*)arr+i));
        }
        break;
        case 2:
        for(i=0; i<tam; i++)
        {
            printf("Elemento %d: \n", i+1);
            scanf("%f", ((float*)arr+i));
        }
        break;
        case 3:
        for(i=0; i<tam; i++)
        {
            printf("Elemento %d: \n", i+1);
            scanf("%c", ((char*)arr+i));
            getchar();
        }
        break;
        case 4:
        for(i=0; i<tam; i++)
        {
            printf("Clave %d: \n", i+1);
            scanf("%d", (&((TAlumno*)arr+i)->clave)); 
        }
    }
}

void busca_elemento(int tipo, void *arr, int tam)
{
    int i;
    int tipo2;
    int entero;
    float flotante;
    char caracter;
    int clave;
    int encontrado;
    printf("Dato que buscas 1 para int, 2 para float y 3 para char y 4 para clave\n");
    scanf("%d", &tipo2);
    switch(tipo2)
    {
        case 1:
        printf("Que numero entero buscas?");
        scanf("%d", &entero);
        for(i=0; i<tam; i++)
        {
            if(entero == *((int*)arr+i))
            {
                encontrado = 1;
                break;
            }
            else
                encontrado = 0;
        }
        if(encontrado == 1)
            printf("Dato encontrado\n");
        else
            printf("Dato no encontrado\n");
        break;
        case 2:
        printf("que dato flotante buscas: ");
        scanf("%f", &flotante);
        for(i=0; i<tam; i++)
        {
            if(flotante == *((float*)arr+i))
            {
                encontrado = 1;
                break;
            }
            else
                encontrado = 0;
        }
        if(encontrado == 1)
            printf("Dato encontrado\n");
        else
            printf("Dato no encontrado\n");
        break;
        case 3:
        printf("que caracter estas buscando: ");
        scanf(" %c", &caracter);
        for(i=0; i<tam; i++)
        {
            if(caracter == *(((char*)arr + i)))
            {
                printf("Dato encontrado\n");
                break;
            }
            else
            {
                printf("dato no encontrado\n");
            }
        }
        break;
        case 4:
        printf("clave que buscas: ");
        scanf("%d", &clave);
        for(i=0; i<tam; i++)
        {
            if(clave == (((TAlumno*)arr + i)->clave))
            {
                encontrado = 1;
                break;
            }
            else
                encontrado = 0;
        }
        if(encontrado == 1)
            printf("Dato encontrado\n");
        else
            printf("Dato no encontrado\n");
        break;
    }
}

void imprime_arreglo(void *arr, int tipo, int tam)
{
    int i;
    switch(tipo)
    {
        case 1:
        for(i = 0; i<tam; i++)
        {
            printf("Elemento %d : %d\n", i+1, *((int*)arr+i));
        }
        break;
        case 2:
        for(i = 0; i<tam; i++)
        {
            printf("Elemento %d : %d\n", i+1, *((float*)arr+i));
        }
        break;
        case 3:
        for(i = 0; i<tam; i++)
        {
            printf("Elemento %d : %c\n", i+1, *((char*)arr+i));
        }
        break;
        case 4:
        for(i=0; i<tam; i++)
        {
            printf("Clave %d: %d\n", i+1, (((TAlumno*)arr+i)->clave));
        }
        
    }
}
