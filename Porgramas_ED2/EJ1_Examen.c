#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct
{
    char sol;
    char *color;
}TOrnato;

typedef struct
{
    char frutal;
    float altura;
}TArbol;

typedef struct
{
    char *nombre;
    int diasriego;
    int existencia;
    char tipo;
    void *generico;
}TPlanta;

TPlanta *crea_espacio(int *n);
void captura(TPlanta *planta, int n);
int cantidad_arb(TPlanta *plantas, int n);
char *color_rojo(TPlanta *plantas, int n);
void imprime(TPlanta *planta, int n);

int main()
{
    int n;
    TPlanta *planta;
    int existencia;
    char *plantas_rojo;
    planta = crea_espacio(&n);
    captura(planta, n);
    existencia = cantidad_arb(planta, n);
    printf("Cantidad de arboles frutales en existencia: %d\n", existencia);
    plantas_rojo = color_rojo(planta, n);
    if (plantas_rojo != NULL)
    {
        printf("Plantas con flores rojas: %s\n", plantas_rojo);
        free(plantas_rojo);
    }
    else
    {
        printf("No hay plantas con flores rojas.\n");
    }
    imprime(planta, n);
    free(planta);
}

TPlanta *crea_espacio(int *n)
{
    TPlanta *plantas;
    printf("Numero de plantas: ");
    scanf("%d", n);
    plantas = (TPlanta*)malloc(sizeof(TPlanta) * (*n));
    if(plantas == NULL)
    {
        printf("No se pudo reservar espacio\n");
        exit(1);
    }
    return plantas;
}

void captura(TPlanta *planta, int n)
{
    int i;
    char cad[100];
    char cad2[100];
    for(i=0; i<n; i++)
    {
        getchar();
        printf("Nombre: ");
        scanf("%[^\n]", cad);
        (planta+i)->nombre = (char*)malloc((strlen(cad) + 1) * sizeof(char));
        strcpy((planta+i)->nombre, cad);
        printf("Dias de riego: ");
        scanf("%d", &(planta+i)->diasriego);
        printf("Existencia: ");
        scanf("%d", &(planta+i)->existencia);
        getchar();
        printf("Tipo (a para arbol, o para ornato): ");
        scanf("%c", &(planta+i)->tipo);
        getchar();
        if((planta+i)->tipo == 'a')
        {
            (planta+i)->generico = malloc(sizeof(TArbol));
            printf("Es frutal? (s/n): ");
            scanf("%c", &(((TArbol*)(planta+i)->generico)->frutal));
            printf("Altura: ");
            scanf("%f", &(((TArbol*)(planta+i)->generico)->altura));
        }
        else if((planta+i)->tipo == 'o')
        {
            (planta+i)->generico = malloc(sizeof(TOrnato));
            printf("Es de sol? (s/n): ");
            scanf("%c", &(((TOrnato*)(planta+i)->generico)->sol));
            getchar();
            printf("De que color es?: ");
            scanf("%[^\n]", cad2);
            ((TOrnato*)(planta+i)->generico)->color = (char*)malloc((strlen(cad2) + 1) * sizeof(char));
            strcpy(((TOrnato*)(planta+i)->generico)->color, cad2);
        }
    }
}

int cantidad_arb(TPlanta *plantas, int n)
{
    int i;
    int existencia=0;
    TArbol *arbol;
    for(i = 0; i < n; i++)
    {
        if((plantas+i)->tipo == 'a') 
        {
            arbol = (TArbol*)(plantas+i)->generico;
            if(arbol->frutal == 's')
            {
                existencia += (plantas+i)->existencia;
            }
        }
    }
    return existencia;  
}   

char *color_rojo(TPlanta *plantas, int n)
{
    int i, cont=0;
    TOrnato *flor;
    char *florRES;
    
    for(i=0; i<n; i++)
    {
        flor=(TOrnato*)(plantas+i)->generico;
        if((plantas+i)->tipo == 'o' && strcmp(flor->color , "rojo")==0)
        {
            cont+=strlen((plantas+i)->nombre);
        }
    }
    florRES=(char*)malloc(sizeof(char)*(cont));
    if(florRES == NULL)
    {
        printf("No se pudo asignar memoria.\n");
        exit(1);
    }
    florRES[0] = '\0';
    for(i=0; i<n; i++)
    {
        if((plantas+i)->tipo == 'o')
        {
            flor=(TOrnato*)(plantas+i)->generico;
            if(strcmp(flor->color , "rojo")==0)
            {
                strcat(florRES, (plantas+i)->nombre);
                strcat(florRES, ",");
            }
        }
    }
    return florRES;
}

void imprime(TPlanta *planta, int n)
{
    int i;
    for(i=0; i<n; i++)
    {
        printf("\nNombre de la planta: %s\n", (planta+i)->nombre);
        printf("Dias de riego: %d\n", (planta+i)->diasriego);
        printf("Numero de existencia: %d\n", (planta+i)->existencia);
        
        if ((planta+i)->tipo == 'a')
        {
            printf("TIPO: ARBOL\n");
            printf("Es frutal?: %c\n", ((TArbol*)(planta+i)->generico)->frutal);
            printf("Altura: %.2f\n", ((TArbol*)(planta+i)->generico)->altura);
        }
        else if ((planta+i)->tipo == 'o')
        {
            printf("TIPO: ORNATO\n");
            printf("Es de sol?: %c\n", ((TOrnato*)(planta+i)->generico)->sol);
            printf("Color: %s\n", ((TOrnato*)(planta+i)->generico)->color);
        }
    }
}