#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct //grafo en matriz
{
    int *vertices;
    int **mR;
    int nv;
    int tipo; // Dirigido o No Dirigido
} TGrafoM; // Grafo en Matriz

typedef struct nodA
{
    //int peso;
    int vertice_d;
    struct nodA *sig;
}TNodoA;

typedef struct nodV
{
    int vertice;
    struct nodA *cab_rel;
}TNodoVV;

typedef struct//grafo en vector relacion o arreglo de listas
{
    TNodoVV *vertices;
    int nv;
    int tipo;  //Dirigido o No Dirigido
}TGrafoV;  //Grafo en vector

typedef struct nodAL
{
    //int peso;
    struct nodVL *arri; //o *dest
    struct nodAL *sig;
}TNodoAL;

typedef struct nodVL
{
    int vertice;
    struct nodVL *sig;
    TNodoAL *aba;
}TNodoVL;

typedef struct//grafo en lista de listas
{
    TNodoVL* grafo;
    int nv;
    int tipo;  //Dirigido o No Dirigido
}TGrafoL;

void crea_grafoM(TGrafoM *g, char nom_arch[]);
int **crea_arreglo2D(int nr, int nc);
void imprime_grafo(TGrafoM g);
void Matriz(TGrafoM g);
int agrega_vertice(TGrafoM *g, int ver);
int modifica_arista(TGrafoM g, int vo, int vd, int va);
void agrega_arista(TGrafoM g, int vo, int vd);
void elimina_arista(TGrafoM g, int vo, int vd);
int elimina_vertice(TGrafoM *g, int ver);

TNodoA *crea_nodoA(int vd);
void inserta_final(TNodoA **cab, int vd);
int elimina_fin(TNodoA **cab, int *vd);
void imprime_lista(TNodoA *cab);
void crea_grafo_vector(TGrafoM g, TGrafoV *g1);
void imprime_grafoV(TGrafoV g);
void modifica_aristaGL(TGrafoV *g, int vo, int vd, int va);

TNodoVL* crea_nodoVL(int vertice);
TNodoAL* crea_nodoAL(TNodoVL *vd);
TNodoVL *inserta_VL(TNodoVL **g, int ver);
TNodoAL *inserta_AL(TNodoAL **cab, TNodoVL *dest);
void crea_grafoLL(TGrafoM g, TGrafoL *g1);
void imprime_grafoLL(TGrafoL g);

int main()
{
    TGrafoM grafo;
    TGrafoV g1;
    TGrafoL g2;
    int v;
    printf("GRAFO MATRIZ:\n");
    crea_grafoM(&grafo, "g1.txt");
    Matriz(grafo);

    /*if (agrega_vertice(&grafo, 6))
    {
        Matriz(grafo);
    }

    printf("Vertice a Eliminar: ");
    scanf("%d", &v);

    if(elimina_vertice(&grafo, v))
    {
        Matriz(grafo);
    }*/

    printf("GRAFO VECTOR:\n");
    crea_grafo_vector(grafo, &g1);
    imprime_grafoV(g1);

    printf("GRAFO LISTAS DE LISTAS\n");
    crea_grafoLL(grafo, &g2);
    imprime_grafoLL(g2);
}

void crea_grafoM(TGrafoM *g, char nom_arch[])
{
    FILE *f;
    char cadena[100];
    int o, d;

    f = fopen(nom_arch, "r");
    if (f == NULL)
    {
        printf("No se puedo abrir el archivo\n");
        return;
    }
    fgets(cadena, 100, f);
    g->nv = strlen(cadena) - 1;
    g->vertices = (int *)malloc(sizeof(int) * g->nv);
    g->mR = crea_arreglo2D(g->nv, g->nv);
    fseek(f, 0, SEEK_SET);
    for (o = 0; o < g->nv; o++)
    {
        *(g->vertices + o) = o + 1; //'A'+o
        fgets(cadena, 100, f);
        for (d = 0; d < g->nv; d++)
        {
            if (cadena[d] == '1')
            {
                *(*(g->mR + o) + d) = 1;
            }
            else
            {
                *(*(g->mR + o) + d) = 0;
            }
        }
    }
    fclose(f);
}

int **crea_arreglo2D(int nr, int nc)
{
    int **a;

    a = (int **)malloc(sizeof(int *) * nr);
    if (a)
    {
        for (int r = 0; r < nr; r++)
        {
            *(a + r) = (int *)malloc(sizeof(int) * nc);
        }
    }
    return (a);
}

void imprime_grafo(TGrafoM g)
{
    int o, d;

    for (o = 0; o < g.nv; o++)
    {
        for (d = 0; d < g.nv; d++)
        {
            if (*(*(g.mR + o) + d))
            {
                printf("%d->%d\n", g.vertices[o], g.vertices[d]);
            }
        }
    }
}

void Matriz(TGrafoM g)
{
    int o, d;

    for (o = 0; o < g.nv; o++)
    {
        printf("%d ", g.vertices[o]);
    }
    printf("\n");
    for (o = 0; o < g.nv; o++)
    {
        for (d = 0; d < g.nv; d++)
        {
            printf("%d ", *(*(g.mR + o) + d));
        }
        printf("\n");
    }
}

int agrega_vertice(TGrafoM *g, int ver)
{
    int b = 1;
    int v;
    // Verifica si el vértice `ver` ya existe en el arreglo `vertices`. Si se encuentra, `b` se pone en 0.
    for (v = 0; v < g->nv && b == 1; v++)
    {
        if (g->vertices[v] == ver)
        {
            b = 0;
        }
    }

    if (b)
    {
        g->nv++;
        g->vertices = (int *)realloc(g->vertices, g->nv * sizeof(int));//agranda 1 espacio en el vertice ej: 1234 ahora agrega 1234[]
        g->vertices[g->nv - 1] = ver;//aqui si agrega el vertice extra ej: 1234 -> 12345
        g->mR = (int **)realloc(g->mR, g->nv * sizeof(int *)); // Agrega un puntero extra en `mR`, representando un nuevo "renglón" en la matriz
        *(g->mR + g->nv - 1) = (int *)malloc(sizeof(int) * g->nv);//asigna la memoria de la fila ej: fila 5 -> | 0 | 0 | 0 | 0 | 0 | 0 |

        for (v = 0; v < g->nv - 1; v++)
        {
            *(g->mR + v) = (int *)realloc(*(g->mR + v), g->nv * sizeof(int)); // Agrega espacio en cada fila (columna adicional)
            *(*(g->mR + v) + g->nv - 1) = 0;//este es para agregar un cero en renglones   
            *(*(g->mR + g->nv - 1) + v) = 0;//este es para agregar un cero en columnas
        }
        *(*(g->mR + g->nv - 1) + v) = 0;
    }
    return b;
}

int modifica_arista(TGrafoM g, int vo, int vd, int va)//va es 0 o 1 para poner o quitar la arista
{
    int b=0;
    int v1, v2;

    for(v1=0; v1<g.nv && b==1; v1++)//busca el vertice origen
    {
        if(g.vertices[v1]==vo)
        {
            b=1;
        }
    }
    if(b)
    {
        b=0;
        for(v2=0; v2<g.nv && b==1; v2++)//busca el vertice destino
        {
            if(g.vertices[v2]==vd)
            {
                b=1;
            }
        }
        if(b)
        {
            *(*(g.mR+v1)+v2)=va;
            if(g.tipo==0)//no dirigido
            {
                *(*(g.mR+v2)+v1)=va;
            } 
        }
    }
    return(b);
}

void agrega_arista(TGrafoM g, int vo, int vd)
{
    modifica_arista(g, vo, vd, 1);
}

void elimina_arista(TGrafoM g, int vo, int vd)
{
    modifica_arista(g, vo, vd, 0);
}

int elimina_vertice(TGrafoM *g, int ver)
{
    int b=0;
    int v, v1;

    for(v=0; v<g->nv && b==0; v++)
    {
        if(g->vertices[v]==ver)
        {
            b=1;
        }
    }
    
    if(b)
    {
        v--;
        g->vertices[v]=g->vertices[g->nv-1];
        g->vertices=(int*)realloc(g->vertices, (g->nv-1) * sizeof(int));
        free(*(g->mR+v));
        *(g->mR+v)=*(g->mR+(g->nv-1));
        g->mR = (int**)realloc(g->mR, (g->nv-1)*sizeof(int*));

        for(v1=0; v1<g->nv-1; v1++)
        {
            *(*(g->mR+v1)+v)=*(*(g->mR+v1)+g->nv-1);
            *(g->mR+v1)=(int*)realloc(*(g->mR+v1), sizeof(int)*g->nv-1);
        }
        g->nv--;
    }
    return v;
}

TNodoA *crea_nodoA(int vd)
{
    TNodoA *aux;

    aux=(TNodoA*)malloc(sizeof(TNodoA));

    if(aux)
    {
        aux->vertice_d=vd;
        aux->sig=NULL;
    }
    return aux;
}

void inserta_final(TNodoA **cab, int vd)
{
    if(!*cab)
    {
        *cab=crea_nodoA(vd);
    }
    else
    {
        inserta_final(&((*cab)->sig), vd);
    }
}

int elimina_fin(TNodoA **cab, int *vd)
{
    int b=0;
    TNodoA *aux=*cab;
    if(*cab != NULL)
    {
        if((*cab)->sig == NULL)
        {
            *cab = NULL;
            *vd = aux->vertice_d;
            b=1;
            free(aux);
        }
        else
        {
            b=elimina_fin(&(*cab)->sig, vd);
        }
    }
    return b;
}

void imprime_lista(TNodoA *cab)
{
    if(cab)
    {
        printf("%d ", cab->vertice_d);
        imprime_lista(cab->sig);
    }
}

void crea_grafo_vector(TGrafoM g, TGrafoV *g1)
{
    int v, v1;

    g1->nv=g.nv;
    g1->vertices=(TNodoVV*)malloc(sizeof(TNodoVV) * g.nv);

    for(v=0; v<g.nv; v++)
    {
        (g1->vertices + v)->vertice = *(g.vertices + v);
        (g1->vertices + v)->cab_rel = NULL;
    }

    for(v=0; v<g.nv; v++)
    {
        for(v1=0; v1<g.nv; v1++)
        {
            if(*(*(g.mR+v)+v1) == 1)
            {
                inserta_final(&((g1->vertices + v)->cab_rel), *(g.vertices+v1));
            }
        }
    }
}

void imprime_grafoV(TGrafoV g)
{
    int o;

    for(o=0; o<g.nv; o++)
    {
        printf("%d -> ", (g.vertices+o)->vertice);
        imprime_lista((g.vertices+o)->cab_rel);
        printf("\n");
    }
}

void modifica_aristaGL(TGrafoV *g, int vo, int vd, int va)//funcion para modificar arista en un arreglo de listas
{
    int v1;

    for(v1=0; v1<g->nv; v1++)
    {
        if((g->vertices+v1)->vertice == vo)
        {
            if(va == 1)
            {
                inserta_final(&(g->vertices+v1)->cab_rel, vd);
            }
            else
            {
                elimina_fin(&(g->vertices+v1)->cab_rel, &vd);
            }
        }
    }
}

TNodoVL* crea_nodoVL(int vertice)
{
    TNodoVL *aux;

    aux=(TNodoVL*)malloc(sizeof(TNodoVL));

    if(aux)
    {
        aux->aba=NULL;
        aux->sig=NULL;
        aux->vertice=vertice;
    }
    return aux;
}

TNodoAL* crea_nodoAL(TNodoVL *vd)
{
    TNodoAL *aux;

    aux=(TNodoAL*)malloc(sizeof(TNodoAL));

    if(aux)
    {
        aux->arri=vd;
        aux->sig=NULL;
    }
    return aux;
}

TNodoVL *inserta_VL(TNodoVL **g, int ver)
{
    TNodoVL *aux=NULL;

    if(*g==NULL)
    {
        aux=crea_nodoVL(ver);
        *g = aux;
    }
    else
    {
        if((*g)->vertice == ver)
        {
            aux=*g;
        }
        else
        {
            aux=inserta_VL(&(*g)->sig, ver);
        }
    }
    return aux;
}

TNodoAL *inserta_AL(TNodoAL **cab, TNodoVL *dest)
{
    if(*cab ==NULL)
    {
        *cab=crea_nodoAL(dest);//este crearia el primer nodo 
    }
    else
    {
        inserta_AL((&(*cab)->sig), dest);//lo inserta al final
    }
}

void crea_grafoLL(TGrafoM g, TGrafoL *g1)
{
    int v, o, d;
    TNodoVL *vo, *vd;
    g1->grafo = NULL;
    g1->nv = g.nv;
    //g1->tipo;

    for(v=0; v < g1->nv; v++)
    {
        inserta_VL(&(g1->grafo), *(g.vertices+v));
    }
    for(o = 0, vo = g1->grafo; o < g1->nv; o++, vo = vo->sig)
    {
        for(d = 0, vd = g1->grafo; d < g1->nv; d++, vd = vd->sig)
        {
            if(*(*(g.mR+o)+d) == 1)
            {
                inserta_AL(&(vo->aba), vd);
            }
        }
    }
}

void imprime_grafoLL(TGrafoL g)
{
    TNodoVL *v;
    TNodoAL *a;

    for(v=g.grafo; v!=NULL; v=v->sig)
    {
        printf("%d -> ", v->vertice);
        for(a=v->aba; a!=NULL; a=a->sig)
        {
            printf("%d ", a->arri->vertice);
        }
        printf("\n");
    }
}