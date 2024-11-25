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
    int visit;
    struct nodVL *sig;
    TNodoAL *aba;
}TNodoVL;

typedef struct//grafo en lista de listas
{
    TNodoVL* grafo;
    int nv;
    int tipo;  //Dirigido o No Dirigido
}TGrafoL;

typedef struct nodo_col {
    TNodoVL *vertice;       
    struct nodo_col *sig;   
}TNodoCol;

typedef struct {
    TNodoCol *frente; 
    TNodoCol *final;  
}TCola;

void crea_grafoM(TGrafoM *g, char nom_arch[]);
int **crea_arreglo2D(int nr, int nc);
void imprime_grafo(TGrafoM g);
void Matriz(TGrafoM g);

TNodoA *crea_nodoA(int vd);
void inserta_final(TNodoA **cab, int vd);
void imprime_lista(TNodoA *cab);
void crea_grafo_vector(TGrafoM g, TGrafoV *g1);
void imprime_grafoV(TGrafoV g);

TNodoVL* crea_nodoVL(int vertice);
TNodoAL* crea_nodoAL(TNodoVL *vd);
TNodoVL *inserta_VL(TNodoVL **g, int ver);
TNodoAL *inserta_AL(TNodoAL **cab, TNodoVL *dest);
void crea_grafoLL(TGrafoM g, TGrafoL *g1);
void imprime_grafoLL(TGrafoL g);

void bfpP(TGrafoL g);
void bpfR(TNodoVL *v);
void bea(TGrafoL g);

void ini_cola(TCola *c);
int cola_vacia(TCola c);
void encolar(TCola *c, TNodoVL *v);
void desencolar(TCola *c, TNodoVL **v);

int main()
{
    TGrafoM grafo;
    TGrafoV g1;
    TGrafoL g2;
    int v;
    printf("GRAFO MATRIZ:\n");
    crea_grafoM(&grafo, "textoTarea7.txt");
    Matriz(grafo);

    printf("GRAFO VECTOR:\n");
    crea_grafo_vector(grafo, &g1);
    imprime_grafoV(g1);

    printf("GRAFO LISTAS DE LISTAS\n");
    crea_grafoLL(grafo, &g2);
    imprime_grafoLL(g2);

    printf("BFP:\n");
    bfpP(g2);

    printf("\nBEA:\n");
    bea(g2);
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
                g->mR[o][d] = 1;
            }
            else
            {
                g->mR[o][d] = 0;
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
            if (((g.mR + o) + d))
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
            printf("%d ", g.mR[o][d]);
        }
        printf("\n");
    }
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
            if(g.mR[v][v1] == 1)
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

TNodoVL* crea_nodoVL(int vertice)
{
    TNodoVL *aux;

    aux=(TNodoVL*)malloc(sizeof(TNodoVL));

    if(aux)
    {
        aux->aba=NULL;
        aux->sig=NULL;
        aux->visit=0;
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
    return *cab;
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
            if(g.mR[o][d] == 1)
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

void bfpP(TGrafoL g)//recorrido en profundidad DFS
{
    TNodoVL *v;

    for(v=g.grafo; v!=NULL; v=v->sig)
    {
        if(v->visit ==0)
        {
            bpfR(v);
        }
    }
}

void bpfR(TNodoVL *v)
{
    TNodoVL *w;
    TNodoAL *a;
    v->visit=1;
    printf("%d -> ", v->vertice);
    for(a=v->aba; a!=NULL; a=a->sig)
    {
        w=a->arri;
        if(w->visit == 0)
        {
            bpfR(w);
        }
    }
}

void bea(TGrafoL g)
{
    TCola c;
    TNodoVL *y, *x;
    TNodoAL *a;
    ini_cola(&c);
    x=g.grafo;
    encolar(&c, x);
    x->visit = 1;
    printf("%d ", x->vertice);
    while(!cola_vacia(c))
    {
        desencolar(&c, &x);
        a=x->aba;
        while(a!=NULL)
        {
            y=a->arri;
            if (y->visit == 0) {  
                encolar(&c, y);    
                y->visit = 1;     
                printf("%d ", y->vertice);  
            }
            a=a->sig;
        }
    }
}

void ini_cola(TCola *c) {
    c->frente = c->final = NULL;  
}

int cola_vacia(TCola c) {
    return c.frente == NULL;  
}

void encolar(TCola *c, TNodoVL *v) {
    TNodoCol *nuevo = (TNodoCol *)malloc(sizeof(TNodoCol));  
    nuevo->vertice = v;
    nuevo->sig = NULL;

    if (cola_vacia(*c)) {
        c->frente = c->final = nuevo;  
    } else {
        c->final->sig = nuevo;  
        c->final = nuevo;       
    }
}

void desencolar(TCola *c, TNodoVL **v) {
    if (!cola_vacia(*c)) {
        TNodoCol *temp = c->frente;  
        *v = temp->vertice;         
        c->frente = c->frente->sig;  
        free(temp);                  
    }
}