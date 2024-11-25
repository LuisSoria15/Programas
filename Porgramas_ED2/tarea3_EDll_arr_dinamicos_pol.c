//tiempo de realizacion 2 horas y media
//Soria Flores Luis Carlos
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


typedef struct
{
    int coeficiente;
    int exponente;
}Polinomio;

void *crea_espacio(int *tam);
void captura_datos(int tam, void *arr);
void evalua(int tam, void *arr);
void deriva(int tam, void *arr);
void libera(void *arr);

int main()
{
    Polinomio polinomio;
    void *arr;
    int tam;
    crea_espacio(&tam);
    captura_datos(tam, arr);
    evalua(tam, arr);
    deriva(tam, arr);
    libera(arr);

}

void *crea_espacio(int *tam)
{
    void *arr;
    printf("Numero de terminos en el polinomio: ");
    scanf("%d", tam);
    arr=malloc(sizeof(Polinomio) * (*tam));
    return arr;
}

void captura_datos(int tam, void *arr)
{
    int i;
    printf("crea tu polinomio:\n");
    for(i = 0; i < tam; i++)
    {
        printf("Escribe tu coeficiente: ");
        scanf("%d", &(((Polinomio*)arr+i)->coeficiente));
        printf("Escribe tu exponente(en caso de no usar, escribe 0): ");
        scanf("%d", &(((Polinomio*)arr+i)->exponente));
    }
}

void evalua(int tam, void *arr)
{
    int i;
    int x, acum=0;
    printf("Valor de x: ");
    scanf("%d", &x);
    for(i=0; i<tam; i++)
    {
        acum += ((Polinomio*)arr+i)->coeficiente * pow(x, ((Polinomio*)arr+i)->exponente);
    }
    printf("Polinomio = %d\n", acum);
}

void deriva(int tam, void *arr)
{
    int i;
    for(i = 0; i < tam; i++)
    {
        Polinomio *pol = ((Polinomio*)arr) + i;
        int coeficiente = pol->coeficiente * pol->exponente;
        int exponente = pol->exponente - 1;
        
        pol->coeficiente = coeficiente;
        pol->exponente = exponente;
    }
    for(i = 0; i < tam; i++)
    {
        Polinomio *pol = ((Polinomio*)arr) + i;
        if(pol->exponente != 0)
        {
            printf("El coeficiente %d es: %d\n", i + 1, pol->coeficiente);
            printf("El exponente %d es: %d\n", i + 1, pol->exponente);
        }
    }
}

void libera(void *arr)
{
    free(arr);
}