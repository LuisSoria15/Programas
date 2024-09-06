#include<stdio.h>
#include<stdlib.h>

typedef struct 
{
    char nombre[30];
    int edad;
    int sexo;
    void *var_g;
}PER;

typedef struct
{
    int cartilla;
}HOMBRE;

typedef struct
{
    int embarazos;
}MUJER;

void *cantidad_trabajadores(int *n);
void captura_info(int n, PER *arr);
void calcula_mujeres_trabajadoras(int n, PER *arr, int *emb ,float *porcentaje);
void imprime_mujeres(float porcentaje, int emb);

int main()
{
    int n;
    PER *arr;
    int emb=0;
    float porcentaje=0.0;
    cantidad_trabajadores(&n);
    captura_info(n, arr);
    calcula_mujeres_trabajadoras(n, arr, &emb, &porcentaje);
    imprime_mujeres(porcentaje, emb);
    free(arr);
}

void *cantidad_trabajadores(int *n)
{
    int i;
     PER *arr;
    printf("Cantidad de trabajadores: \n");
    scanf("%d", n);
    arr=(PER*)malloc(sizeof(PER) * (*n));
    
    if(arr==NULL)
    {
        printf("No se pudo reservar espacio");
        exit(1);
    }
    return(arr);
}

void captura_info(int n, PER *arr)
{
    int i;
    for(i=0; i<n; i++)
    {
        getchar();
        printf("Nombre: ");
        scanf("%[^\n]", (arr+i)->nombre);
        printf("Edad: ");
        scanf("%d", &(arr+i)->edad);
        printf("Sexo 1 Hombre, 2 Mujer: ");
        scanf("%d", &(arr+i)->sexo);

        if((arr+i)->sexo == 1)
        {
            (arr+i)->var_g=malloc(sizeof(HOMBRE));
            printf("numero de cartilla militar: ");
            scanf("%d", &(((HOMBRE*)(arr+i)->var_g)->cartilla));
        }
        else
        {
            (arr+i)->var_g=malloc(sizeof(MUJER));
            printf("Numero de embarazos: ");
            scanf("%d", &(((MUJER*)(arr+i)->var_g)->embarazos));
        }
    }
}

void calcula_mujeres_trabajadoras(int n, PER *arr, int *emb, float *porcentaje) {
    int i, cont = 0;
    for (i = 0; i < n; i++) 
    {
        if ((arr + i)->sexo == 2) 
        {
            cont++;
            if (((MUJER*)(arr + i)->var_g)->embarazos >= 1) 
            {
                (*emb)++;
            }
        }
    }
    *porcentaje = ((float)cont / n) * 100;
}

void imprime_mujeres(float porcentaje, int emb)
{
    printf("Porcentaje mujeres embarazadas: %.2f\n", porcentaje);
    printf("Numero de embarazadas: %d", emb);
}