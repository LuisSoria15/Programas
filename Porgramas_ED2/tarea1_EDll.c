//Tiempo de realizacion: 3 horas
//Soria Flores Luis Carlos
//Estructuras de datos ll
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TAM 30

typedef struct
{
  char nombre[TAM];
  char direccion[TAM];
  int telefono;
  int dia;
  int mes;
  int anio;
  char tipo[TAM];
  char alergias[TAM];
  char observaciones[TAM];

}PACIENTE;

int registro(PACIENTE *p, int *cont);
int imprime_paciente_por_nombre(PACIENTE *p, int cont);
int fecha_ultima_visita_por_nombre(PACIENTE *p, int cont);
int lista_alergias(PACIENTE *p, int cont);
int imprime_todos_pacientes(PACIENTE *p, int cont);

int main()
{
    PACIENTE paciente[TAM];
    PACIENTE *p;
    int cont=0;
    p = paciente;
    registro(p, &cont);
    imprime_paciente_por_nombre(p, cont);
    fecha_ultima_visita_por_nombre(p, cont);
    lista_alergias(p, cont);
    imprime_todos_pacientes(p, cont);
}


int registro(PACIENTE *p, int *cont)
{
    printf("Los nombres y direccion se ingresan sin espacios\n\n\n");
    int opcion;
    do {
        printf("Ingresa tu nombre: ");
        scanf("%s", (p + *cont)->nombre);
        printf("Ingresa tu direccion: ");
        scanf("%s", (p + *cont)->direccion);
        printf("Ingresa tu telefono: ");
        scanf("%d", &(p + *cont)->telefono);
        printf("Fecha ultima visita (dia/mes/anio):\n");
        scanf("%d %d %d", &(p + *cont)->dia, &(p + *cont)->mes, &(p + *cont)->anio);
        printf("¿Privado?");
        scanf("%s", (p + *cont)->tipo);
        printf("Tiene alguna alergia?: ");
        scanf("%s", (p + *cont)->alergias);
        printf("Observaciones: ");
        scanf(" %[^\n]", (p + *cont)->observaciones);
        getchar();


        (*cont)++;

        printf("Desea ingresar otro paciente? (1 si, 0 no): ");
        scanf("%d", &opcion);
    } while(opcion != 0 && *cont < TAM);

    return 0;
}


int imprime_paciente_por_nombre(PACIENTE *p, int cont)
{
    char nombre[TAM];
    int i;
    int opcion;
    do{
        printf("Ingresa el nombre de un paciente para ver todos sus datos:\n");
        scanf("%s", nombre);

        for(i=0; i < cont; i++)
        {
            if(strcmp(nombre, (p + i)->nombre)==0)
            {
                printf("Nombre: %s,\nDireccion: %s\n", (p + i)->nombre, (p + i)->direccion);
                printf("Telefono: %d\nFecha ultima cita:\nDia: %d Mes: %d Año: %d\n", (p + i)->telefono, (p + i)->dia, (p + i)->mes, (p + i)->anio);
                printf("Privado: %s\nAlergias: %s\nObservaciones: %s",(p + i)->tipo, (p + i)->alergias, (p + i)->observaciones);
                printf("\n");
            }
        }
        printf("Desea buscar otro?: 1 si 0 no\n");
        scanf("%d", &opcion);
    }while(opcion != 0);
}

int fecha_ultima_visita_por_nombre(PACIENTE *p, int cont)
{
    char nombre[TAM];
    int i;
    printf("Ingresa el nombre de un paciente para ver su ultima cita:\n");
    scanf("%s", nombre);

    for(i=0; i < cont; i++)
    {
        if(strcmp(nombre, (p + i)->nombre)==0)
        {
            printf("ULTIMA CITA\nDia:%d\nMes:%d\nAnio:%d\n\n", (p + i)->dia, (p + i)->mes, (p + i)->anio);
            return 0;
        }
    }
    printf("Paciente no encontrado.\n");
}

int lista_alergias(PACIENTE *p, int cont)
{
    int i;
    printf("PACIENTES CON ALERGIAS:\n");
    for(i=0; i<cont; i++)
    {
        if(strcmp((p + i)->alergias, "si")==0)
        {
            printf("nombre %d: %s\n", i, (p + i)->nombre);
        }
    }
    printf("\n");
}

int imprime_todos_pacientes(PACIENTE *p, int cont)
{
    int i;
    printf("LISTADO DE TODOS LOS PACIENTES:\n");
    for(i = 0; i < cont; i++)
    {
        printf("Nombre: %s,\nDireccion: %s\n", (p + i)->nombre, (p + i)->direccion);
        printf("Telefono: %d\nFecha ultima cita:\nDia: %d Mes: %d Año: %d\n", (p + i)->telefono, (p + i)->dia, (p + i)->mes, (p + i)->anio);
        printf("Privado:\nAlergias: %s\nObservaciones:%s\n",(p + i)->tipo, (p + i)->alergias, (p + i)->observaciones);
        printf("\n");
    }
}