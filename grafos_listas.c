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

/*typedef struct//grafo en lista de listas
{
    TNodoVL* grafo;
    int nv;
    int tipo;  //Dirigido o No Dirigido
}TGrafoL;*/

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
void crea_grafo_vector(TGrafoM g, TGrafoV *g1);

int main()
{
    TGrafoM grafo;

    crea_grafoM(&grafo, "g1.txt");
    Matriz(grafo);
    if (agrega_vertice(&grafo, 6));
    Matriz(grafo);
    //  imprime_grafo(grafo);
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
    printf("\n");

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
                inserta_final(&((g1->vertices + v)->cab_rel), 'A' + v1);
            }
        }
    }
}