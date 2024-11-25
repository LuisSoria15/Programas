//Tiempo de realizacion 2 horas
//Luis Carlos Soria Flores
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef struct nodP
{
    char nom[20];
    float precio;
    int stock;
    struct nodP *sig;
}TNodoP;

typedef struct nodC
{
    char nombre[20];
    struct nodC *sig;
    TNodoP *aba;
}TNodoC;

typedef struct ll
{
    TNodoC *cat;
    TNodoP *prod;
}TNodoL;

void menu();
TNodoP *creaNodProd(char *nom, float precio, int stock);
TNodoC *creaNodCaT(char *nombre);
TNodoC *inserta_Cat(TNodoC **l, char *categoria);
void inserta_Prod(TNodoP **l, char *producto, float precio, int stock);
void inserta_LL(TNodoC **l, char *categoria,char *producto, float precio, int stock);
TNodoC *busca_Cat(TNodoC *l, char *cat);
void imprime(TNodoC *l);

int main()
{
    menu();
    return 0;
}

void menu() {
    int opcion;
    TNodoC *l = NULL;
    char prod[20];
    float precio;
    int stock;
    char cat[20];
    do {
        printf("1-Para ingresar categoria\n");
        printf("2-Para ingresar producto a categoria\n");
        printf("3-Para listar el inventario\n");
        printf("0-Para salir\n");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) 
        {
        case 1:
            printf("Ingresa el nombre de una categoria:\n");
            scanf("%s", cat);
            getchar();
            inserta_Cat(&l, cat);
            break;
        case 2:
            printf("A que categoria quieres ingresar el producto:\n");
            scanf("%s", cat);
            getchar();
            printf("Inserta producto:\n");
            scanf("%s", prod);
            getchar();
            printf("Inserta el precio:\n");
            scanf("%f", &precio);
            printf("Inserta la cantidad en stock:\n");
            scanf("%d", &stock);
            inserta_LL(&l, cat, prod, precio, stock);
            break;
        case 3:
            imprime(l);
            break;
        case 0:
            printf("Saliendo\n");
            break;
        default:
            printf("Opcion no valida.\n");
            break;
        }
    } while (opcion != 0);
}

TNodoP *creaNodProd(char *nom, float precio, int stock)
{
    TNodoP *aux;
    aux=(TNodoP*)malloc(sizeof(TNodoP));

    if(aux)
    {
        strcpy(aux->nom, nom);
        aux->precio=precio;
        aux->stock=stock;
        aux->sig=NULL;
    }
    return aux;
}

TNodoC *creaNodCaT(char *nombre)
{
    TNodoC *aux;
    aux=(TNodoC*)malloc(sizeof(TNodoC));

    if(aux)
    {
        strcpy(aux->nombre, nombre);
        aux->sig=NULL;
        aux->aba=NULL;
    }
    return aux;
}

TNodoC *inserta_Cat(TNodoC **l, char *categoria) 
{
    TNodoC *aux;
    aux=busca_Cat(*l, categoria);
    if(aux)
    {
        return aux;
    }
    
    if (!(*l) || strcmp((*l)->nombre, categoria) > 0) 
    {
        aux = creaNodCaT(categoria); 
        aux->sig = (*l);        
        (*l) = aux;           
    } 
    else 
    {
        aux = inserta_Cat(&((*l)->sig), categoria);
    }
    return aux;
}

void inserta_Prod(TNodoP **l, char *producto, float precio, int stock)
{
    TNodoP *aux;

    if(!*l || (*l)->precio > precio)
    {
        aux=creaNodProd(producto, precio, stock);
        aux->sig = *l;
        *l = aux;
    }
    else
    {
        inserta_Prod(&((*l)->sig), producto, precio, stock);
    }
}

void inserta_LL(TNodoC **l, char *categoria,char *producto, float precio, int stock)
{
    TNodoC *aux;

    if(!*l)
    {
        *l=creaNodCaT(categoria);
        (*l)->aba=creaNodProd(producto, precio, stock);
    }
    else
    {
        aux = busca_Cat(*l, categoria);
        if(!aux)
        {
            aux=inserta_Cat(l, categoria);
        }
        inserta_Prod(&aux->aba, producto, precio, stock);
    }
}

TNodoC *busca_Cat(TNodoC *l, char *cat)
{
    TNodoC *aux=l;
    while(aux)
    {
        if(strcmp(cat, aux->nombre)==0)
        {
            return aux;
        }
        aux=aux->sig;
    }
    return NULL;
}

void imprime(TNodoC *l)
{
    TNodoC *corre1 = l;
    TNodoP *corre2;

    while(corre1)
    {
        printf("Categoria: %s\n", corre1->nombre);
        corre2=corre1->aba;
        if (!corre2) 
        {
            printf("No hay productos en esta categoria.\n");
        }
        else
        {
            while(corre2)
            {
                printf("Producto: %s\n", corre2->nom);
                printf("Precio: %.2f\n", corre2->precio);
                printf("Stock: %d\n", corre2->stock);
                corre2=corre2->sig;
            }
        }
        corre1=corre1->sig;
    }
}