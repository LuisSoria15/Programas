#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//al momento de crear entidad podemos crear los n atributos que queramos y ahi ahcer la conexion entre ellos
#define MAIN_ENTITY_POINTER 0
#define EMPTY_POINTER -1
#define DATA_BLOCK_SIZE 50

typedef struct Entity
{
    char name[DATA_BLOCK_SIZE];
    long dataPointer;
    long attributesPointer;
    long nextEntity;
}ENTITY;

typedef struct Attribute
{
    char name[DATA_BLOCK_SIZE];
    bool isPrimary;
    long type;
    long size;
    long nextAttribute;
}ATTRIBUTE;

void createEntity(FILE* dataDictionary);
void CreateAttribute(FILE* dataDictionary, ENTITY currentEntity);
FILE* initializeDataDictionary(const char *dictionaryName);
int appendEntity(FILE *dataDictionary, ENTITY newEntity);
void reorderEntities(FILE* dataDictionary, long currentEntityPointer, const char* NewEntityName, long newEntityDirection);
ENTITY removeEntities(FILE* dataDictionary, long currentEntityPointer , const char* EntityName);
int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute);
void reorderAttributes(FILE *dataDictionary, long currentAttributePointer, const char*newAttributeName, long newAttributeDirection);
void menu();
void SetPointerEntity(FILE *dataDictionary, ENTITY *currentEntity, long header, const char* entityName);
void printEntitiesWithAttributes(FILE *dataDictionary, long header);

int main(int argc, char** argv)
{
    menu();
    return 0;
}

void menu()
{
    char name[8];
    printf("\nEnter a dictionary Name: ");
    fgets(name, sizeof(name), stdin);//scanf
    name[strcspn(name, "\n")] = '\0';
    FILE* dataDictionary = initializeDataDictionary(name);
    int opcion;
    char charEntity[10];

    do
    {
        printf("\nMenu:");
        printf("\n1 - Alta de entidad");
        printf("\n2 - Alta de atributo");
        printf("\n3 - Listar entidades y atributos");
        printf("\n4 - Ingresar datos a un atributo");
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
            //currentEntity.attributesPointer=66;
            CreateAttribute(dataDictionary, currentEntity);
            break;
        case 3:
            printEntitiesWithAttributes(dataDictionary, MAIN_ENTITY_POINTER);
            break;
        case 4:
            printf("Ingresa una entidad: ");
            fgets(charEntity, sizeof(charEntity), stdin);
            charEntity[strcspn(charEntity, "\n")] = '\0';
            ENTITY currentEntity;
            SetPointerEntity(dataDictionary, &currentEntity, MAIN_ENTITY_POINTER, charEntity);
            //funcion para crear datos
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

FILE* initializeDataDictionary(const char *dictionaryName)
{
    long mainHeader = EMPTY_POINTER;//inicializa la variable main header con -1

    printf("Initializing Data Dictionary...\n");

    FILE *dictionary = fopen(dictionaryName, "w+");

    fwrite(&mainHeader, sizeof(mainHeader), 1, dictionary);//escribe la variable en el archivo de tamaño long(8bytes) [-1]

    return dictionary;
}

void createEntity(FILE* dataDictionary)
{
    ENTITY newEntity;

    printf("\nEnter the entity name:");//memset
    memset(newEntity.name, '\0', sizeof(newEntity.name));
    fgets(newEntity.name, sizeof(newEntity.name), stdin);//hace un scanf de teclado de la entidad a usar por ejemplo "alumnos"
    newEntity.name[strcspn(newEntity.name, "\n")] = '\0';

    newEntity.dataPointer = EMPTY_POINTER;//inicializa lcada parte de la estructura en -1
    newEntity.attributesPointer = EMPTY_POINTER;
    newEntity.nextEntity = EMPTY_POINTER;

    long entityDirection = appendEntity(dataDictionary, newEntity);//entra append con el archivo y newEntitiy que es la variable de la estructura
    printf("Returned entityDirection: %ld\n", entityDirection);

    reorderEntities(dataDictionary, MAIN_ENTITY_POINTER, newEntity.name, entityDirection);//entra con el archivo, MAIN que vale 0, el nombre de la entidad y los bytes de la primer posicion
}

int appendEntity(FILE *dataDictionary, ENTITY newEntity)
{
    if (dataDictionary == NULL) 
    {
        perror("File pointer is NULL");
        return -1;
    }

    fseek(dataDictionary, 0 , SEEK_END);//se posiciona en la posicion 0 que vale [-1] al inicio del archivo o dependiendo la iteracion
    long entityDirection = ftell(dataDictionary);//a la variable entityDirection se le asigna ese dato donde se posiciono el puntero -1

    fwrite(newEntity.name, DATA_BLOCK_SIZE, 1, dataDictionary);//en la posicion 8 se inserta el nombre con 50 bytes
    fwrite(&newEntity.dataPointer, sizeof(long), 1, dataDictionary);//en la posicion 58 se inserta el puntero de datos con 8 bytes
    fwrite(&newEntity.attributesPointer, sizeof(long), 1, dataDictionary);//en la posicion 66 se inserta el puntero de att con 8 bytes
    fwrite(&newEntity.nextEntity, sizeof(long), 1, dataDictionary);//en la posicion 74 se inserta el next con 8 bytes
    //visualizacion de una sola entidad [-1][alumnos][-1][-1][-1]
    //                                  0   4       54  58  62  66

    printf("Entity written at %ld", entityDirection);

    return entityDirection;//se regresa el tamaño de 8 bytes(en la priemra iteracion) o los que haya segun el caso
}

void reorderEntities(FILE* dataDictionary, long currentEntityPointer, const char* newEntityName, long newEntityDirection)
{
    long currentEntityDirection = -1;

    fseek(dataDictionary, currentEntityPointer, SEEK_SET);//currentEntityPtr siempre sera cero, asi que va al inicio de todo
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);//lee que byte esta dentro de esa posicion

    if(currentEntityDirection == -1L)//aqui solo aplica si esta vacio o es la primera iteracion
    {
        fseek(dataDictionary, currentEntityPointer, SEEK_SET);//aqui de nuevo va al inicio de todo
        fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);//aqui modifica la cabecera inicial con el numero de bytes en el que esta ubicado el atributo
    }
    else
    {
        char currentEntityName[DATA_BLOCK_SIZE];//creamos variables auxiliares
        long nextEntityDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);//se posiciona en donde el byte que tenia el main header
        fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);//copia el nombre de la entidad de esa ubicacion
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) *2);//en la variable nextheaderptr mete el byte donde se encuentra la estructura nextHeaderptr

        if(strcmp(currentEntityName, newEntityName) < 0)//compara la entidad nueva que creamos con la que estamos comparando para saber si es mayor
        {
            reorderEntities(dataDictionary, nextHeaderPointer, newEntityName, newEntityDirection);//en caso de ser mayor, entra recursivamente para ir buscando donde entrar de manera ordenada
        }
        else
        {
            fseek(dataDictionary, currentEntityPointer, SEEK_SET);//en esta parte se va al inicio de bytes a 0 al main header al inicio de todo
            fwrite(&newEntityDirection, sizeof(long), 1, dataDictionary);//aqui actualiza la nueva cabecera, ej: si antes era 8, ahora es 82
            fseek(dataDictionary, newEntityDirection + DATA_BLOCK_SIZE + (sizeof(long) * 2), SEEK_SET);//aqui ahora busca el nextHeaderptr de la estructura de la entidad
            fwrite(&currentEntityDirection, sizeof(long), 1, dataDictionary);//y aqui pone como siguente el que remplazo,ej: antes -1 ahora 8 dependiendo el caso
        }
    }
}

/*ENTITY removeEntities(FILE* dataDictionary, long currentEntityPointer , const char* EntityName)
{
    long currentEntityDirection = -1;
    
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityDirection, sizeof(currentEntityDirection), 1, dataDictionary);

    if(currentEntityDirection == -1L)
    {
        return 0;
    }
    else
    {
        ENTITY resultEntity;
        long nextEntityDirection;
        long nextHeaderPointer;

        fseek(dataDictionary, currentEntityDirection, SEEK_SET);
        fread(resultEntity.name, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);
        nextHeaderPointer = ftell(dataDictionary) + (sizeof(long) *2);

        if(strcmp(resultEntity.name, EntityName) == 0)
        {
            fread(&resultEntity.dataPointer, sizeof(long), 1, dataDictionary);
            fread(&resultEntity.attributesPointer, sizeof(long), 1, dataDictionary);
            fread(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);

            fseek(dataDictionary, currentEntityPointer, SEEK_SET);
            fwrite(&resultEntity.nextEntity, sizeof(long), 1, dataDictionary);

            return resultEntity;
        }
        else
        {
            return removeEntities(dataDictionary, nextHeaderPointer, EntityName);
        }
    }
}*/

void CreateAttribute(FILE* dataDictionary, ENTITY currentEntity)
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

    newAttribute.nextAttribute= EMPTY_POINTER;

    long attributeDirection = appendAttribute(dataDictionary, newAttribute);
    reorderAttributes(dataDictionary, currentEntity.attributesPointer, newAttribute.name, attributeDirection);

}

int appendAttribute(FILE *dataDictionary, ATTRIBUTE newAttribute)
{
    fseek(dataDictionary, 0 , SEEK_END);//va al final del archivo
    long entityDirection = ftell(dataDictionary);//mete el valor de bytes del final del archivo

    fwrite(newAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
    fwrite(&newAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
    fwrite(&newAttribute.type, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.size, sizeof(long), 1, dataDictionary);
    fwrite(&newAttribute.nextAttribute, sizeof(long), 1, dataDictionary);//en todos los fwrite, añade al archivo todos los datos

    return entityDirection;//devuelve la posicion de inicio de donde se insertaron los datos
}

void reorderAttributes(FILE *dataDictionary, long currentAttributePointer, const char* newAttributeName, long newAttributeDirection)
{
    long currentAttributeDirection = -1;

    fseek(dataDictionary, currentAttributePointer, SEEK_SET);
    fread(&currentAttributeDirection, sizeof(currentAttributeDirection), 1, dataDictionary);

    if(currentAttributeDirection == -1L)
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

        if(strcmp(currentAttributeName, newAttributeName) < 0)
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

void SetPointerEntity(FILE *dataDictionary, ENTITY *currentEntity, long currentEntitiesDirection, const char* entityName)
{
    long nextHeaderPointer;
    long currentEntityPointer;
    char currentEntityName[DATA_BLOCK_SIZE];

    fseek(dataDictionary, currentEntitiesDirection, SEEK_SET);
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary);
    fseek(dataDictionary, currentEntityPointer, SEEK_SET);
    fread(&currentEntityName, sizeof(char), DATA_BLOCK_SIZE, dataDictionary);

    if (strcmp(entityName, currentEntityName) == 0) 
    {
        currentEntity->dataPointer = ftell(dataDictionary);
        currentEntity->attributesPointer = ftell(dataDictionary)+sizeof(long);
        return;
    }
    else
    {
        long newcurrentEntitiesDirection;
        fseek(dataDictionary, currentEntityPointer + DATA_BLOCK_SIZE + sizeof(long) * 2, SEEK_SET);
        newcurrentEntitiesDirection = ftell(dataDictionary);
        SetPointerEntity(dataDictionary, currentEntity, newcurrentEntitiesDirection, entityName);
    }
}

void printEntitiesWithAttributes(FILE *dataDictionary, long header)
{
    long currentEntityPointer;
    fseek(dataDictionary, header, SEEK_SET);
    fread(&currentEntityPointer, sizeof(long), 1, dataDictionary);

    printf("\n----------List of Entities and their Attributes:---------------\n");
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
            while (currentAttributePointer != EMPTY_POINTER)
            {
                ATTRIBUTE currentAttribute;
                fseek(dataDictionary, currentAttributePointer, SEEK_SET);
                fread(currentAttribute.name, DATA_BLOCK_SIZE, 1, dataDictionary);
                fread(&currentAttribute.isPrimary, sizeof(bool), 1, dataDictionary);
                fread(&currentAttribute.type, sizeof(long), 1, dataDictionary);
                fread(&currentAttribute.size, sizeof(long), 1, dataDictionary);
                fread(&currentAttribute.nextAttribute, sizeof(long), 1, dataDictionary);


                printf("    - Name of attribute: %s", currentAttribute.name);
                printf("      Primary Key: %s\n", currentAttribute.isPrimary ? "Yes" : "No");
                printf("      Type: %ld\n", currentAttribute.type);
                printf("      Size: %ld\n", currentAttribute.size);


                currentAttributePointer = currentAttribute.nextAttribute;//aqui avanza al otro atributo
            }
        }


        currentEntityPointer = currentEntity.nextEntity;//aqui avanza a la otra entidad
    }
}

void readAttributeType()//esta funcion leera el tipo de atributo desde la direccion de la entidad para poder asignar la informacion correctamente con condiciones
{

}

void readData()//esta funcion hara el scanf necesario para cada tipo de atributo con un switch
{

}

void appendData()//esta funcion hara el fwrite en el archivo y lo conectara dataPointer de la entidad seleccionada
{

}