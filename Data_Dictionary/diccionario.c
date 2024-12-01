#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//definiciones
#define MAIN_ENTITY_POINTER 0
#define EMPTY_POINTER -1
#define DATA_BLOCK_SIZE 50
//estructuras 
typedef struct Entity
{
    char name[DATA_BLOCK_SIZE]; // nombre de mi entidad
    long dataPointer;           // apunta a los datos, despues de pasar por un proceso de validacion con los atributos
    long attributesPointer;     // escribe el nombre del tipo de dato ej: nombre, apellido, clave, calificacion
    long nextEntity;            // contiene la direccion de la siguiente entidad en bytes
} ENTITY;

typedef struct Attribute
{
    char name[DATA_BLOCK_SIZE]; // nombre de mi atributo
    bool isPrimary;             // vefica la clave a usar, se tiene que comprobar de un conjunto de att que sea el unico con 1 y se ordenen en base al dato que seleccione(no se usara en el programa actual por tiempo)
    long type;                  // tipo de dato, entero, flotante, cadena
    long size;                  // tamaño de bloques que se usara
    long nextAttribute;         // apuntador en bytes al siguiente atributo
} ATTRIBUTE;

// inicializacion
FILE *initializeDataDictionary(const char *dictionaryName);
FILE *initializeDataDictionary2(const char *dictionaryName);
// menus
void menu(FILE *dataDictionary);
void menu2();
// funciones entidad
void createEntity(FILE *dataDictionary);
int appendEntity(FILE *dataDictionary, ENTITY newEntity);
void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *NewEntityName, long newEntityDirection);
int removeEntities(FILE *dataDictionary, long currentEntityPointer, const char *EntityName);
// funciones attributo
void CreateAttribute(FILE *dataDictionary, ENTITY currentEntity);
int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute);
void reorderAttributes(FILE *dataDictionary, long currentAttributePointer, const char *newAttributeName, long newAttributeDirection);
int removeAttributes(FILE *dataDictionary, long currentAttributesPointer, const char *attName);
// funciones auxiliares
void SetPointerEntity(FILE *dataDictionary, ENTITY *currentEntity, long header, const char *entityName);
int ReadAttLocation(FILE *dataDictionary, ENTITY *currentEntity2);
void readAttributeType(FILE *dataDictionary, long attributeDirection, ENTITY *currentEntity2);
void setPointerToData(FILE *dataDictionary, long EndOFile, ENTITY *currentEntity2, long attributeDirection);
//impresion
void printEntitiesWithAttributesAndData(FILE *dataDictionary, long header);
void printEntityAtt(FILE *dataDictionary, long header);


int main(int argc, char **argv)
{
    menu2();
    return 0;
}

void menu(FILE *dataDictionary)
{
    int opcion;
    char charEntity[10];
    char charAttribute[10];

    do
    {
        printf("\nMenu Opciones:");
        printf("\n1 - Alta de entidad");
        printf("\n2 - Alta de atributo");
        printf("\n3 - Listar entidades y atributos");
        printf("\n4 - Ingresar datos a una Entidad");
        printf("\n5 - Dar de baja una entidad");
        printf("\n6 - Dar de baja un atributo");
        printf("\n0 - Salir");
        printf("\nElige una opcion: ");
        scanf("%d", &opcion);
        getchar();
        switch (opcion)
        {
        case 1:
            createEntity(dataDictionary);
            break;
        case 2:
            printf("Ingresa una entidad: ");
            fgets(charEntity, sizeof(charEntity), stdin);
            charEntity[strcspn(charEntity, "\n")] = '\0';
            ENTITY currentEntity;
            SetPointerEntity(dataDictionary, &currentEntity, MAIN_ENTITY_POINTER, charEntity);
            CreateAttribute(dataDictionary, currentEntity);
            break;
        case 3:
            printEntityAtt(dataDictionary, MAIN_ENTITY_POINTER);
            break;
        case 4:
            printf("Ingresa una entidad: ");
            fgets(charEntity, sizeof(charEntity), stdin);
            charEntity[strcspn(charEntity, "\n")] = '\0';
            ENTITY currentEntity2;
            SetPointerEntity(dataDictionary, &currentEntity2, MAIN_ENTITY_POINTER, charEntity); // busca la entidad escrita
            long attDirection = ReadAttLocation(dataDictionary, &currentEntity2);               // entrega la direccion del primer tipo de atributo
            fseek(dataDictionary, 0, SEEK_END);
            long endFile = ftell(dataDictionary);                                     // me entrega el final del archivo y el principio del atributo
            readAttributeType(dataDictionary, attDirection, &currentEntity2);         // aqui vamos a hacer insercion al final del file de los datos, basados en nuestros atributos
            setPointerToData(dataDictionary, endFile, &currentEntity2, attDirection); // primero hace la conexion de dataPointer de entity al fin del file por que ahi se insertaran lso datos de la siguiente funcion
            break;
        case 5:
            printf("Ingresa una entidad a eliminar: ");
            fgets(charEntity, sizeof(charEntity), stdin);
            charEntity[strcspn(charEntity, "\n")] = '\0';
            removeEntities(dataDictionary, MAIN_ENTITY_POINTER, charEntity);
            break;
        case 6:
            printf("Ingresa una entidad: ");
            fgets(charEntity, sizeof(charEntity), stdin);
            charEntity[strcspn(charEntity, "\n")] = '\0';
            ENTITY currentEntity3;
            SetPointerEntity(dataDictionary, &currentEntity3, MAIN_ENTITY_POINTER, charEntity);
            printf("Ingresa un atributo a eliminar: ");
            fgets(charAttribute, sizeof(charAttribute), stdin);
            charAttribute[strcspn(charAttribute, "\n")] = '\0';
            long currentAttDir = currentEntity3.attributesPointer;
            removeAttributes(dataDictionary, currentAttDir, charAttribute);
            break;
        case 0:
            break;
        default:
            printf("Opción no válida. Intenta de nuevo.\n");
            break;
        }
    } while (opcion != 0);

    fclose(dataDictionary);
}

void menu2()
{
    int opcion;
    char name[15];
    do
    {
        printf("*****MENU PRINCIPAL*****\n");
        printf("1- Crear Diccionario nuevo\n");
        printf("2- Abrir Diccionario Existente\n");
        printf("0- Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        FILE *dataDictionary = NULL;
        switch (opcion)
        {
        case 1:
            printf("Nombre del diccionario: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            dataDictionary = initializeDataDictionary(name);
            if (dataDictionary)
            {
                menu(dataDictionary);
                fclose(dataDictionary);
            }
            break;
        case 2:
            printf("Nombre del diccionario Existente: ");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = '\0';
            dataDictionary = initializeDataDictionary2(name);
            if (dataDictionary)
            {
                menu(dataDictionary);
                fclose(dataDictionary);
            }
            else
            {
                printf("Error al abrir el diccionario.\n");
            }
            break;
        case 0:
            printf("Saliendo...\n");
            break;
        default:
            printf("Ingresa una opción válida\n");
            break;
        }
    } while (opcion != 0);
}

FILE *initializeDataDictionary(const char *dictionaryName)
{
    long mainHeader = EMPTY_POINTER; // inicializa la variable main header con -1
    printf("Initializing Data Dictionary...\n");
    FILE *dictionary = fopen(dictionaryName, "w+");
    fwrite(&mainHeader, sizeof(mainHeader), 1, dictionary); // escribe la variable en el archivo de tamaño long(8bytes) [-1]
    return dictionary;
}

FILE *initializeDataDictionary2(const char *dictionaryName)
{
    printf("Initializing Existing Data Dictionary...\n");
    FILE *dictionary = fopen(dictionaryName, "r+");
    if (!dictionary)
    {
        printf("No se pudo abrir el archivo: %s\n", dictionaryName);
    }
    return dictionary;
}

void createEntity(FILE *dataDictionary)
{
    ENTITY newEntity;

    printf("\nEnter the entity name:"); // memset
    memset(newEntity.name, '\0', sizeof(newEntity.name));
    fgets(newEntity.name, sizeof(newEntity.name), stdin); // hace un scanf de teclado de la entidad a usar por ejemplo "alumnos"
    newEntity.name[strcspn(newEntity.name, "\n")] = '\0';

    newEntity.dataPointer = EMPTY_POINTER; // inicializa lcada parte de la estructura en -1
    newEntity.attributesPointer = EMPTY_POINTER;
    newEntity.nextEntity = EMPTY_POINTER;

    long entityDirection = appendEntity(dataDictionary, newEntity); // entra append con el archivo y newEntitiy que es la variable de la estructura
    printf("Returned entityDirection: %ld\n", entityDirection);

    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection); // entra con el archivo, MAIN que vale 0, el nombre de la entidad y los bytes de la primer posicion
}

int appendEntity(FILE *dataDictionary, ENTITY newEntity)
{
    if (dataDictionary == NULL)
    {
        perror("File pointer is NULL");
        return -1;
    }

    fseek(dataDictionary, 0, SEEK_END);           // se posiciona en la posicion 0 que vale [-1] al inicio del archivo o dependiendo la iteracion
    long entityDirection = ftell(dataDictionary); // a la variable entityDirection se le asigna ese dato donde se posiciono el puntero -1

    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);            // en la posicion 8 se inserta el nombre con 50 bytes
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary);       // en la posicion 58 se inserta el puntero de datos con 8 bytes
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary); // en la posicion 66 se inserta el puntero de att con 8 bytes
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary);        // en la posicion 74 se inserta el next con 8 bytes
    // visualizacion de una sola entidad [-1][alumnos][-1][-1][-1]
    //                                   0   4       54  58  62  66

    printf("\nEntity written at:  %ld\n", entityDirection);

    return entityDirection; // se regresa el tamaño de 8 bytes(en la priemra iteracion) o los que haya segun el caso
}

void reorderEntities(FILE *dataDictionary, long currentEntityPointer, const char *newEntityName, long newEntityDirection)
{
    long currentEntityDirection = -1;

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);                             // currentEntityPtr siempre sera cero, asi que va al inicio de todo
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary); // lee que byte esta dentro de esa posicion

    if (currentEntityDirection == -1L) // aqui solo aplica si esta vacio o es la primera iteracion
    {
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);        // aqui de nuevo va al inicio de todo
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary); // aqui modifica la cabecera inicial con el numero de bytes en el que esta ubicado el atributo
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE]; // creamos variables auxiliares
        long nextEntityDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);                  // se posiciona en donde el byte que tenia el main header
        fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary); // copia el nombre de la entidad de esa ubicacion
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);           // en la variable nextheaderptr mete el byte donde se encuentra la estructura nextHeaderptr

        if (strcmp(currentEntityName, newEntityName) < 0) // compara la entidad nueva que creamos con la que estamos comparando para saber si es mayor
        {
            reorderEntities(dataDictionary, nextHeaderPointer, newEntityName, newEntityDirection); // en caso de ser mayor, entra recursivamente para ir buscando donde entrar de manera ordenada
        }
        else
        {
            fseek(dataDictionary, currentEntityPointer, SEEK_SET);                                      // en esta parte se va al inicio de bytes a 0 al main header al inicio de todo
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);                               // aqui actualiza la nueva cabecera, ej: si antes era 8, ahora es 82
            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + (sizeof(long) * 2), SEEK_SET); // aqui ahora busca el nextHeaderptr de la estructura de la entidad
            fwrite(&currentEntityDirection, sizeof(long), 1, dataDictionary);                           // y aqui pone como siguente el que remplazo,ej: antes -1 ahora 8 dependiendo el caso
        }
    }
}

int removeEntities(FILE *dataDictionary, long currentEntityPointer, const char *EntityName)
{
    long currentEntityDirection = -1;

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if (currentEntityDirection == -1L)
    {
        return EXIT_FAILURE;
    }
    else
    {
        ENTITY resultEntity;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(resultEntity.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) * 2);

        if (strcmp(resultEntity.name, EntityName) == 0)
        {
            fread(&resultEntity.dataPointer, sizeof(long), 1, dataDictionary);
            fread(&resultEntity.attributesPointer, sizeof(long), 1, dataDictionary);
            fread(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);
            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);
            return EXIT_SUCCESS;
        }
        else
        {
            return removeEntities(dataDictionary, nextHeaderPointer, EntityName);
        }
    }
}

void CreateAttribute(FILE *dataDictionary, ENTITY currentEntity)
{
    ATTRIBUTE newAttribute;

    printf("\nEnter the Attribute name:");
    memset(newAttribute.name, '\0', sizeof(newAttribute.name));
    fgets(newAttribute.name, sizeof(newAttribute.name), stdin);
    newAttribute.name[strcspn(newAttribute.name, "\n")] = '\0';

    printf("\nIs this attribute a primary key? (1: Yes, 0: No): ");
    scanf("%d", &newAttribute.isPrimary);
    getchar();
    printf("\nEnter the type of the attribute (1: int, 2: float, 3: char, 4: bool): ");
    scanf("%ld", &newAttribute.type);
    getchar();
    printf("\nEnter the size of the attribute (0 if not applicable): ");
    scanf("%ld", &newAttribute.size);
    getchar();

    newAttribute.nextAttribute = EMPTY_POINTER;

    long attributeDirection = appendAttribute(dataDictionary, newAttribute); // hara append al final del archivo, attDir vale la direccion del final
    reorderAttributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);

    long test;
    fseek(dataDictionary, currentEntity.attributesPointer, SEEK_SET);
    fread(&test, sizeof(long), 1, dataDictionary);
    printf(" Direccion del primer atributo escrita en: %ld\n", test);
}

int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute)
{
    fseek(dataDictionary, 0, SEEK_END);           // va al final del archivo
    long entityDirection = ftell(dataDictionary); // mete el valor de bytes del final del archivo

    fwrite(newAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
    fwrite(&newAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
    fwrite(&newAttribute.type, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.nextAttribute, sizeof(long), 1, dataDictionary); // en todos los fwrite, añade al archivo todos los datos

    return entityDirection; // devuelve la posicion de inicio de donde se insertaron los datos
}

void reorderAttributes(FILE *dataDictionary, long currentAttributePointer, const char *newAttributeName, long newAttributeDirection)
{
    long currentAttributeDirection = -1;

    fseek(dataDictionary, currentAttributePointer, SEEK_SET);
    fread(&currentAttributeDirection, sizeof(currentAttributeDirection), 1, dataDictionary);

    if (currentAttributeDirection == -1L)
    {
        fseek(dataDictionary, currentAttributePointer, SEEK_SET);
        fwrite(&newAttributeDirection, sizeof(currentAttributeDirection), 1, dataDictionary);
    }
    else
    {
        char currentAttributeName[DATA_BLOCK_SIZE];
        long nextAttributeDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentAttributeDirection, SEEK_SET);
        fread(&currentAttributeName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + sizeof(bool) + (sizeof(long) * 2);

        if (strcmp(currentAttributeName, newAttributeName) < 0)
        {
            reorderAttributes(dataDictionary, nextHeaderPointer, newAttributeName, newAttributeDirection);
        }
        else
        {
            fseek(dataDictionary, currentAttributePointer, SEEK_SET);
            fwrite(&newAttributeDirection, sizeof(long), 1, dataDictionary);
            fseek(dataDictionary, newAttributeDirection + DATA_BLOCK_SIZE + sizeof(bool) + (sizeof(long) * 2), SEEK_SET);
            fwrite(&currentAttributeDirection, sizeof(long), 1, dataDictionary);
        }
    }
}

int removeAttributes(FILE *dataDictionary, long currentAttributesPointer, const char *attName)
{
    long currentAttributeDirection = -1;

    fseek(dataDictionary, currentAttributesPointer, SEEK_SET);          // nos posicionamos en el bloque de direccion de entidad a att
    fread(&currentAttributeDirection, sizeof(long), 1, dataDictionary); // comprobamos si existe algun atributo

    if (currentAttributeDirection == -1L)
    {
        return EXIT_FAILURE;
    }
    else
    {
        ATTRIBUTE resultAtt;
        long nextHeaderPointer;

        fseek(dataDictionary, currentAttributeDirection, SEEK_SET);
        fread(resultAtt.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + sizeof(bool) + (sizeof(long) * 2);

        if (strcmp(resultAtt.name, attName) == 0)
        {
            fread(&resultAtt.isPrimary, sizeof(bool), 1, dataDictionary);
            fread(&resultAtt.type, sizeof(long), 1, dataDictionary);
            fread(&resultAtt.size, sizeof(long), 1, dataDictionary);
            fread(&resultAtt.nextAttribute, sizeof(long), 1, dataDictionary);
            fseek(dataDictionary, currentAttributesPointer, SEEK_SET);
            fwrite(&resultAtt.nextAttribute, sizeof(long), 1, dataDictionary);
            return EXIT_SUCCESS;
        }
        else
        {
            return removeAttributes(dataDictionary, nextHeaderPointer, attName);
        }
    }
}

void SetPointerEntity(FILE *dataDictionary, ENTITY *currentEntity, long currentEntitiesDirection, const char *entityName)
{ // pasamos como parametros del diccionario, la struct entity la variable currentEntitiesDir(que simpre valdra 0 en la 1era iteracion que es el inicio del file), y el nombre de la entidad a buscar
    long nextHeaderPointer;
    long currentEntityPointer;
    char currentEntityName[DATA_BLOCK_SIZE];

    fseek(dataDictionary, currentEntitiesDirection, SEEK_SET);     // nos posicionamos al inicio de todo el file
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary); // leemos el byte que esta ahi(debe haber datos)
    if (currentEntityPointer == -1L)                               // en caso de ser -1 el header, significa que o no hay conexion alguna o no hay entidades
    {
        printf("No existen enitdades");
        return;
    }
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);                    // despues de leer el dato aqui nos posicionamos a la direccion de la 1era entidad
    fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary); // leemos el nombre de la entidad y la guardamos

    if (strcmp(entityName, currentEntityName) == 0) // si es igual a la que buscamos
    {
        currentEntity->dataPointer = ftell(dataDictionary);                      // entonces aqui asignamos a la struct su respectivo apuntador para poder usarlo en otras funciones(solo da la direccion de donde esta, no el dato interno)
        currentEntity->attributesPointer = ftell(dataDictionary) + sizeof(long); // mismo aqui, podemos usar el apuntador a datos y attributos de la entidad que necesitamos(solo da la direccion de donde esta, no el dato interno)
        return;
    }
    else
    {
        long newcurrentEntitiesDirection;
        fseek(dataDictionary, currentEntityPointer + DATA_BLOCK_SIZE + sizeof(long) * 2, SEEK_SET);
        newcurrentEntitiesDirection = ftell(dataDictionary); // en caso de no encontrar la entidad llamamos recursivamente con el apuntador como "header" a la sigueinte entidad a buscar y asi hasta encontrarse
        SetPointerEntity(dataDictionary, currentEntity, newcurrentEntitiesDirection, entityName);
    }
}

void printEntitiesWithAttributesAndData(FILE *dataDictionary, long header)
{
    long currentEntityPointer;
    fseek(dataDictionary, header, SEEK_SET);
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary);

    printf("\n----------List of Entities, Attributes, and Data:---------------\n");
    while (currentEntityPointer != EMPTY_POINTER)
    {
        ENTITY currentEntity;

        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fread(currentEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
        fread(&currentEntity.dataPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.attributesPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.nextEntity, sizeof(long), 1, dataDictionary);

        printf("\nEntity: %s", currentEntity.name);

        long currentAttributePointer = currentEntity.attributesPointer;
        if (currentAttributePointer == EMPTY_POINTER)
        {
            printf("  No attributes\n");
        }
        else
        {
            printf("  -----Attributes------\n");
            long totalAttributeSize = 0; // Total size of all attributes
            ATTRIBUTE attributes[100];   // Store attributes for later data reading
            int attributeCount = 0;

            while (currentAttributePointer != EMPTY_POINTER)
            {
                ATTRIBUTE currentAttribute;
                fseek(dataDictionary, currentAttributePointer, SEEK_SET);
                fread(currentAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
                fread(&currentAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
                fread(&currentAttribute.type, sizeof(long), 1, dataDictionary);
                fread(&currentAttribute.size, sizeof(long), 1, dataDictionary);
                fread(&currentAttribute.nextAttribute, sizeof(long), 1, dataDictionary);

                printf("    - Name of attribute: %s\n", currentAttribute.name);
                printf("      Primary Key: %s\n", currentAttribute.isPrimary ? "Yes" : "No");
                printf("      Type: %ld\n", currentAttribute.type);
                printf("      Size: %ld\n", currentAttribute.size);

                attributes[attributeCount++] = currentAttribute;
                totalAttributeSize += currentAttribute.size;
                currentAttributePointer = currentAttribute.nextAttribute;
            }

            // Print Data Section
            if (currentEntity.dataPointer != EMPTY_POINTER)
            {
                printf("  -----Data------\n");
                fseek(dataDictionary, currentEntity.dataPointer, SEEK_SET);
                char *dataBuffer = malloc(totalAttributeSize);

                while (fread(dataBuffer, totalAttributeSize, 1, dataDictionary) == 1)
                {
                    char *dataCursor = dataBuffer;
                    for (int i = 0; i < attributeCount; ++i)
                    {
                        ATTRIBUTE attr = attributes[i];
                        printf("    - %s: ", attr.name);
                        switch (attr.type)
                        {
                        case 1: // Integer
                            printf("%d", *(int *)dataCursor);
                            break;
                        case 2: // Float
                            printf("%f", *(float *)dataCursor);
                            break;
                        case 3: // String
                            printf("%s", (char *)dataCursor);
                            break;
                        case 4: // Boolean
                            printf("%s", (*(int *)dataCursor) ? "true" : "false");
                            break;
                        default:
                            printf("Unknown type");
                            break;
                        }
                        dataCursor += attr.size;
                        printf("\n");
                    }
                    printf("\n");
                }

                free(dataBuffer);
            }
            else
            {
                printf("  No data for this entity.\n");
            }
        }

        currentEntityPointer = currentEntity.nextEntity; // Move to the next entity
    }
}

void printEntityAtt(FILE *dataDictionary, long header)
{
    long currentEntityPointer;

    fseek(dataDictionary, header, SEEK_SET);
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary);

    printf("\n------Entidades y Atributos------\n");
    while (currentEntityPointer != EMPTY_POINTER)
    {
        ENTITY currentEntity;
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);
        fread(currentEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);
        fread(&currentEntity.dataPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.attributesPointer, sizeof(long), 1, dataDictionary);
        fread(&currentEntity.nextEntity, sizeof(long), 1, dataDictionary);
        printf("\nEntity: %s\n", currentEntity.name);

        long currentAttPointer = currentEntity.attributesPointer;
        if (currentAttPointer == EMPTY_POINTER)
        {
            printf("No hay atributos en la entidad\n");
        }
        while (currentAttPointer != EMPTY_POINTER)
        {
            ATTRIBUTE currentAttribute;
            fseek(dataDictionary, currentAttPointer, SEEK_SET);
            fread(currentAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
            fread(&currentAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
            fread(&currentAttribute.type, sizeof(long), 1, dataDictionary);
            fread(&currentAttribute.size, sizeof(long), 1, dataDictionary);
            fread(&currentAttribute.nextAttribute, sizeof(long), 1, dataDictionary);
            printf("    - Nombre del atributo: %s\n", currentAttribute.name);
            printf("      Llave primaria: %s\n", currentAttribute.isPrimary ? "Si" : "No");
            printf("      Tipo: %ld\n", currentAttribute.type);
            printf("      Tamanio: %ld\n", currentAttribute.size);
            currentAttPointer = currentAttribute.nextAttribute;
        }
        currentEntityPointer = currentEntity.nextEntity;
    }
}

int ReadAttLocation(FILE *dataDictionary, ENTITY *currentEntity2)
{
    long attLocation;

    fseek(dataDictionary, currentEntity2->attributesPointer, SEEK_SET); // aqui nos posicionamos en el espacio donde esta el aputnador al atributo, puede se -1 o que si haya atributo
    fread(&attLocation, sizeof(long), 1, dataDictionary);               // leemos el no. que tiene, sea -1 o la direccion

    if (attLocation == -1L)
    {
        printf("No hay atributos en esta entidad");
        return 0;
    }

    return attLocation; // return de la direccion del primer atributo de la entidad
}

void readAttributeType(FILE *dataDictionary, long attributeDirection, ENTITY *currentEntity2)
{
    char currentAttributeName[DATA_BLOCK_SIZE]; // guardara el nombre del atributo para mostrar en msj
    long type;
    long currentPos;
    long customSize;

    memset(currentAttributeName, '\0', DATA_BLOCK_SIZE);

    // Lee el nombre del atributo
    fseek(dataDictionary, attributeDirection, SEEK_SET);
    fread(&currentAttributeName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);

    // Lee el tipo de atributo
    fseek(dataDictionary, attributeDirection + DATA_BLOCK_SIZE + sizeof(bool), SEEK_SET);
    fread(&type, sizeof(long), 1, dataDictionary);

    // Leer el tamaño del atributo
    fseek(dataDictionary, attributeDirection + DATA_BLOCK_SIZE + sizeof(bool) + sizeof(long), SEEK_SET);
    fread(&customSize, sizeof(long), 1, dataDictionary);

    void *value = malloc(customSize);
    memset(value, '\0', customSize);

    printf("Ingresa un valor para: %s\n", currentAttributeName);
    switch (type)
    {
    case 1:
        printf("Ingresa un valor Tipo entero: ");
        scanf("%d", (int *)value);
        getchar();
        break;
    case 2:
        printf("Ingresa un valor Tipo flotante: ");
        scanf("%f", (float *)value);
        getchar();
        break;
    case 3:
        printf("Ingresa un valor Tipo cadena: ");
        scanf("%s", (char *)value);
        getchar();
        break;
    case 4:
        printf("Ingresa un valor tipo booleano 1 verdadero 0 falso:");
        scanf("%d", (int *)value);
        getchar();
        break;
    default:
        printf("Tipo no reconocido\n");
        free(value);
        break;
    }

    // append al file
    fseek(dataDictionary, 0, SEEK_END);
    fwrite(value, customSize, 1, dataDictionary);

    free(value);

    // Lee el siguiente atributo
    long nextAtt;
    fseek(dataDictionary, attributeDirection + DATA_BLOCK_SIZE + sizeof(bool) + sizeof(long) * 2, SEEK_SET);
    fread(&nextAtt, sizeof(long), 1, dataDictionary);
    if (nextAtt == -1L)
    {
        long empty = -1;
        fseek(dataDictionary, 0, SEEK_END);
        fwrite(&empty, sizeof(long), 1, dataDictionary); // inserta al final del archivo un -1
    }
    else
    {
        readAttributeType(dataDictionary, nextAtt, currentEntity2);
    }
}

void setPointerToData(FILE *dataDictionary, long EndOFile, ENTITY *currentEntity2, long attributeDirection)
{ // pasa como parametro el diccionario, la direccion del fin del archivo y la struct endity con ya datos de direccion de una entidad dentro

    long dataPoint;
    fseek(dataDictionary, currentEntity2->dataPointer, SEEK_SET); // nos posicionamos en el apuntador de atributo de al entidad
    fread(&dataPoint, sizeof(long), 1, dataDictionary);           // leemos el contenido de la direccion de dataPointer de la entidad, puede ser -1 o ya tener alguna direccion a datos

    if (dataPoint == -1L)
    {
        fseek(dataDictionary, currentEntity2->dataPointer, SEEK_SET);
        fwrite(&EndOFile, sizeof(long), 1, dataDictionary); // en caso de no tener alguna direccion, se la asignamos al final del file donde deberia estar los datos
    }
    else
    {
        int BytesCount = 0; // vamos a leer el tamaño de todo el bloque de datos
        long currentAttPointer;
        fseek(dataDictionary, currentEntity2->attributesPointer, SEEK_SET); // nos posicionamos en att pointer de la entidad
        fread(&currentAttPointer, sizeof(long), 1, dataDictionary);         // leemos donde esta posicionado la entidad

        while (currentAttPointer != EMPTY_POINTER) // ciclo para moverse entre entidades
        {
            ATTRIBUTE currentAttribute;
            fseek(dataDictionary, currentAttPointer + DATA_BLOCK_SIZE + sizeof(bool) + sizeof(long), SEEK_SET); // nos posicionamos en el primer atributo de la entidad
            fread(&currentAttribute.size, sizeof(long), 1, dataDictionary);                                     // leemos el bloque de datos para saber cuanto vale
            BytesCount = BytesCount + currentAttribute.size;                                                    // acumulamos en una variable ese bloque de datos
            fread(&currentAttribute.nextAttribute, sizeof(long), 1, dataDictionary);                            // leemos la dir siguiente atributo para saber a donde desplazarnos
            currentAttPointer = currentAttribute.nextAttribute;                                                 // nos movemos al siguiente atributo hasta que su bloque de siguiente sea -1
        }

        long nextDataDir = EMPTY_POINTER;
        long nextDataPtr = currentEntity2->dataPointer;
        fseek(dataDictionary, currentEntity2->dataPointer, SEEK_SET); // nos posicionamos en el apuntador de atributo de al entidad
        fread(&nextDataDir, sizeof(long), 1, dataDictionary);         // leemos el contenido de la direccion de dataPointer de la entidad, puede ser -1 o ya tener alguna direccion a datos

        while (nextDataDir != EMPTY_POINTER)
        {
            fseek(dataDictionary, nextDataDir + BytesCount, SEEK_SET); // aqui nos posicionamos en el final del bloque de datos
            nextDataPtr = ftell(dataDictionary);
            fread(&nextDataDir, sizeof(long), 1, dataDictionary);
        }

        fseek(dataDictionary, nextDataPtr, SEEK_SET);       // aqui nos posicionamos en el final del bloque de datos
        fwrite(&EndOFile, sizeof(long), 1, dataDictionary); // y escribimos su direccion, para tener esa conexion entre datos
    }
}
