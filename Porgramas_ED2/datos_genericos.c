#include<stdio.h>
#include<stdlib.h>
#include <malloc.h>
#include<string.h>
#define N 1

typedef struct
{
    int clave;
    char nombre[30];
}TAlumno;


void *crea_dato_generico(int *tipo);
void captura_dato_generico(void *ptr, int tipo);
void imprime_dato_generico(void *ptr, int tipo);

int main()
{
    TAlumno TAlumno;
    int tipo;
    void *ptr;
    ptr = crea_dato_generico(&tipo);
    captura_dato_generico(ptr, tipo);
    imprime_dato_generico(ptr, tipo);
}

void *crea_dato_generico(int *tipo)
{
    void *ptr;
    int tam;
    printf("Tipo(1 int 2 float 3 char 4 cadena 5 alumno) \n");
    scanf("%d", tipo);
    switch (*tipo)
    {
    case 1:ptr=malloc(sizeof(int));
        break;
    case 2:ptr=malloc(sizeof(float));
        break;
    case 3:ptr=malloc(sizeof(char));
        break;
    case 4:printf("Introduce el tamanio de la cadena: ");
            scanf("%d", &tam);
            ptr=malloc(sizeof(char)*tam);
        break;
    case 5:ptr=malloc(sizeof(TAlumno));
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
        break;
    case 3: printf("caracter");
            scanf("%c", (char*)ptr);
            getchar();
        break;
    case 4: printf("cadena: ");
            scanf("%s", (char*)ptr);
            getchar();
        break;
    case 5: printf("clave: ");
            scanf("%d", &(((TAlumno*)ptr)->clave));
            getchar();
            printf("nombre: ");
            scanf("%s", ((TAlumno*)ptr)->nombre);
            getchar();
        break;
    }
}

void imprime_dato_generico(void *ptr, int tipo)
{
    /*
    TAlumno *pa;
    int *pi;
    char *pc;
    float *pf;


    dentro de los casos seria:
    case 1: pi=(int*)ptr;
    */
    switch(tipo)
    {
    case 1: 
            printf("entero: %d", *((int*)ptr));
        break;
    case 2: 
            printf("Flotante: %f", *((float*)ptr));
        break;
    case 3: 
            printf("caracter: %c", *((char*)ptr));
        break;
    case 4: 
            printf("cadena: %s", ((char*)ptr));//no lleva * por que es un arreglo, apuntador a cadena
        break;
    case 5: 
            printf("clave: %d\n", ((TAlumno*)ptr)->clave);
            printf("nombre: %s", ((TAlumno*)ptr)->nombre);
        break;
    }
}