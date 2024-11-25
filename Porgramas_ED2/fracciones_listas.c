//luis carlos soria flores
//tiempo de realizacion: 2 horas
#include <stdio.h>
#include <stdlib.h>

typedef struct Fraccion
{
    int num;
    int den;
    struct Fraccion *sig;
} TNodoFraccion;

typedef struct Mixta
{
    int entero;
    int num;
    int den;
    struct Mixta *sig;
} TNodoMixta;

TNodoFraccion *crea_fraccion(int num, int den);
TNodoMixta *crea_mixta(int ent, int num, int den);
void inserta_frac_inicio(TNodoFraccion **cab, int num, int den);
void inserta_frac_final(TNodoFraccion **cab, int num, int den);
void inserta_fracmix_inicio(TNodoMixta **cab, int num, int den, int Pent);
void inserta_fracmix_final(TNodoMixta **cab, int num, int den, int Pent);
void lee_frac(TNodoFraccion **cab, int *elem);
void suma_frac(TNodoFraccion *cab, int *numRES, int *denRES);
void mixtas(TNodoFraccion *cab, TNodoMixta **cab2);
void imprime_frac(TNodoFraccion *cab);
void imprime_fracmix(TNodoMixta *cab2);
int mcd(int num, int num2);
void simp_frac(int *num, int *den);
void suma_frac_simp(TNodoFraccion *cab, int *numRES, int *denRES);

int main()
{
    TNodoFraccion *cabecera = NULL;
    TNodoMixta *cabecera2 = NULL;
    int elem;
    int num, den;
    int numRES, denRES;
    printf("dime el numero de fracciones en tu lista:\n");
    scanf("%d", &elem);
    lee_frac(&cabecera, &elem); // por si acaso fuera a usar n elementos
    suma_frac(cabecera, &numRES, &denRES);
    printf("Resultado de la suma de fracciones sin simplificar es: %d/%d\n", numRES, denRES);
    suma_frac_simp(cabecera, &numRES, &denRES);
    printf("El resultado de la suma de fracciones simplificada es: %d/%d\n", numRES, denRES);
    mixtas(cabecera, &cabecera2);
    printf("\ntodas las fraccones:\n");
    imprime_frac(cabecera);
    printf("Fraciones impropias representadas en mixta: \n");
    imprime_fracmix(cabecera2);
}

TNodoFraccion *crea_fraccion(int num, int den)
{
    TNodoFraccion *aux;
    aux = (TNodoFraccion *)malloc(sizeof(TNodoFraccion));

    if (aux)
    {
        aux->num = num;
        aux->den = den;
        aux->sig = NULL;
    }
    return aux;
}

TNodoMixta *crea_mixta(int ent, int num, int den)
{
    TNodoMixta *aux;
    aux = (TNodoMixta *)malloc(sizeof(TNodoMixta));

    if (aux)
    {
        aux->entero = ent;
        aux->den = den;
        aux->num = num;
        aux->sig = NULL;
    }
    return aux;
}

void inserta_frac_inicio(TNodoFraccion **cab, int num, int den)
{
    TNodoFraccion *aux;

    aux = crea_fraccion(num, den);
    if (aux != NULL)
    {
        aux->sig = *cab;
        *cab = aux;
    }
}

void inserta_frac_final(TNodoFraccion **cab, int num, int den)
{
    TNodoFraccion *aux, *corre;
    aux=crea_fraccion(num, den);
    if(aux)
    {
        //*cab = NULL;(no me funcionaba con esto)
        if(*cab == NULL)
        {
            *cab =aux;
        }
        else
        {
            corre = *cab;
            while(corre->sig != NULL)
            {
                corre=corre->sig;
            }
            corre->sig=aux;
        }
    }
}

void inserta_fracmix_inicio(TNodoMixta **cab, int num, int den, int Pent)
{
    TNodoMixta *aux;

    aux = crea_mixta(Pent, num, den);
    if (aux != NULL)
    {
        aux->sig = *cab;
        *cab = aux;
    }
}

void inserta_fracmix_final(TNodoMixta **cab, int num, int den, int Pent)
{
    TNodoMixta *aux, *corre;
    aux = crea_mixta(Pent, num, den);
    if(aux)
    {
        //*cab = NULL;(no me funcionaba con esto)
        if(*cab == NULL)
        {
            *cab =aux;
        }
        else
        {
            corre = *cab;
            while(corre->sig != NULL)
            {
                corre=corre->sig;
            }
            corre->sig=aux;
        }
    }
}

void lee_frac(TNodoFraccion **cab, int *elem)
{
    int num, den;
    for (int i = 0; i < *elem; i++)
    {
        printf("Numerador %d: ", i + 1);
        scanf("%d", &num);
        printf("Denominador %d: ", i + 1);
        scanf("%d", &den);
        inserta_frac_final(cab, num, den);
    }
}

void suma_frac(TNodoFraccion *cab, int *numRES, int *denRES)
{
    TNodoFraccion *aux = cab;
    *denRES = 1, *numRES = 0;
    int res;
    while (aux != NULL)
    {
        *denRES = *denRES * aux->den;
        aux = aux->sig;
    }
    aux = cab;
    while (aux != NULL)
    {
        *numRES += (*denRES / aux->den) * aux->num;
        aux = aux->sig;
    }
}

int mcd(int num, int num2) 
{
    while (num2 != 0) 
    {
        int temp = num2;
        num2 = num % num2;
        num = temp;
    }
    return num;
}

void simp_frac(int *num, int *den) 
{
    int div = mcd(*num, *den);
    *num = *num / div;
    *den = *den / div;
}

void suma_frac_simp(TNodoFraccion *cab, int *numRES, int *denRES)
{
    TNodoFraccion *aux = cab;
    *denRES = 1, *numRES = 0;
    int res;
    while (aux != NULL)
    {
        *denRES = *denRES * aux->den;
        aux = aux->sig;
    }
    aux = cab;
    while (aux != NULL)
    {
        *numRES += (*denRES / aux->den) * aux->num;
        aux = aux->sig;
    }
    simp_frac(numRES, denRES);
}

void mixtas(TNodoFraccion *cab, TNodoMixta **cab2)
{
    TNodoFraccion *aux = cab;
    int resid = 0, mix = 0;
    while (aux != NULL)
    {
        if (aux->num > aux->den)
        {
            resid = aux->num % aux->den;
            mix = aux->num / aux->den;
            inserta_fracmix_final(cab2, resid, aux->den, mix);
        }
        aux = aux->sig;
    }
}

void imprime_frac(TNodoFraccion *cab)
{
    TNodoFraccion *aux=cab;
    while (aux != NULL)
    {
        printf("%d/%d\n", aux->num, aux->den);
        aux=aux->sig;
    }
}

void imprime_fracmix(TNodoMixta *cab2)
{
    TNodoMixta *aux=cab2;
    while(aux != NULL)
    {
        printf("%d %d/%d\n", aux->entero, aux->num, aux->den);
        aux=aux->sig;
    }
}