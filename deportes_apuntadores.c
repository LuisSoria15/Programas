#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct 
{
    char *nom;
    int nvict;
    int nderr;
    int tipo_d;
    void *dato_v;
}TEquipo;

typedef struct 
{
    int bolbat;
    int carr;
    int errores;
    int juegoex;
}TB;

typedef struct
{
    int emp;
    int tant;
    int gol_c;
    int contraa;
    int yd;
    int ydop;
}TF;  

TEquipo *crea_espacio(int *ne);
void captura(int ne, TEquipo *eqs);
void imprime(int ne, TEquipo *eqs);

int main()
{
    int ne;
    void *eqs;
    crea_espacio(&ne);
    captura(ne, eqs);
    imprime(ne, eqs);
}

TEquipo *crea_espacio(int *ne)
{
    TEquipo *eqs;
    printf("numero de equipo: ");
    scanf("%d", ne);

    eqs = (TEquipo*)malloc(sizeof(TEquipo)*(*ne));
    if(eqs==NULL)
    {
        printf("No se pudo reservar espacio");
        exit(1);
    }
    return eqs;
}

void captura(int ne, TEquipo *eqs)
{
    int i;
    char cadena[100];
    TB *dv;
    TF *dv2;
    for(i=0; i<ne; i++)
    {
        getchar();
        printf("Nombre y ciudad del equipo:\n");
        scanf("%[^\n]", cadena);
        (eqs+i)->nom=(char*)malloc(strlen(cadena)*sizeof(char*));
        strcpy((eqs+i)->nom, cadena);
        printf("Numero de victorias:\n");
        scanf("%d", &(eqs+i)->nvict);
        printf("Numero de derrotas: \n");
        scanf("%d", &(eqs+i)->nderr);
        printf("Tipo de deporte: 1 B. 2 F.\n");
        scanf("%d", &(eqs+i)->tipo_d);
        if((eqs+i)->tipo_d == 1)
        {
            (eqs+i)->dato_v=malloc(sizeof(TB));
            dv = (TB*)(eqs+i)->dato_v;
            printf("Bolas Bateadas:\n");
            scanf("%d", &(dv->bolbat));
            printf("Numero de carreras:\n");
            scanf("%d", &(dv->carr));
            printf("Numero de errores: \n");
            scanf("%d", &(((TB*)(eqs+i)->dato_v)->errores));
            printf("Numero de juegos extra: \n");
            scanf("%d", &(((TB*)(eqs+i)->dato_v)->juegoex));
        }
        else
        {
            (eqs+i)->dato_v=malloc(sizeof(TF));
            printf("Numero de empates: \n");
            scanf("%d", &(((TF*)(eqs+i)->dato_v)->emp));
            printf("Numero de tantos: \n");
            scanf("%d", &(((TF*)(eqs+i)->dato_v)->tant));
            printf("numero de goles de campo: \n");
            scanf("%d", &(((TF*)(eqs+i)->dato_v)->gol_c));
            printf("numero de yardas ganadas: \n");
            scanf("%d", &(((TF*)(eqs+i)->dato_v)->yd));
            printf("Numero de yardas cedidas: \n");
            scanf("%d", &(((TF*)(eqs+i)->dato_v)->ydop));
        }
    }
}

void imprime(int ne, TEquipo *eqs)
{
    int i;
    TB *dv;
    TF *dv2;
    int tipo;
    for(i = 0; i<ne; i++)
    {
        printf("Datos a imprimir: 1 TB 2 TF\n");
        scanf("%d", &(eqs+i)->tipo_d);
        if((eqs+i)->tipo_d == 1)
        {
            printf("Nombre y ciudad del equipo: %s\n", (eqs+i)->nom);
            printf("Numero de victorias:%d\n", (eqs+i)->nvict);
            printf("Numero de derrotas:%d\n", (eqs+i)->nderr);
            if((eqs+i)->tipo_d == 1)
            {
                printf("Deporte: BEISBOL\n");
                printf("Bolas Bateadas:%d\n", ((TB*)(eqs+i)->dato_v)->bolbat);
                printf("Numero de carreras:%d\n", ((TB*)(eqs+i)->dato_v)->carr);
                printf("Numero de errores:%d \n", ((TB*)(eqs+i)->dato_v)->errores);
                printf("Numero de juegos extra: %d\n", ((TB*)(eqs+i)->dato_v)->juegoex);
            }
        }
        else
        {
            printf("Nombre y ciudad del equipo:%s\n", (eqs+i)->nom);
            printf("Numero de victorias:%d\n", (eqs+i)->nvict);
            printf("Numero de derrotas:%d \n", (eqs+i)->nderr);
            if((eqs+i)->tipo_d == 2)
            {
                printf("deporte: FUTBOL\n");
                printf("Numero de empates:%d \n", ((TF*)(eqs+i)->dato_v)->emp);
                printf("Numero de tantos:%d \n", ((TF*)(eqs+i)->dato_v)->tant);
                printf("numero de goles de campo:%d \n", ((TF*)(eqs+i)->dato_v)->gol_c);
                printf("numero de yardas ganadas:%d \n", ((TF*)(eqs+i)->dato_v)->yd);
                printf("Numero de yardas cedidas:%d \n", ((TF*)(eqs+i)->dato_v)->ydop);
            }
            
        }
    }
}