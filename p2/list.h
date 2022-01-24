/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef P0_LIST_H
#define P0_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOM 50
#define MAXDIR 500

typedef struct {
    void *dir;
    int size;
    char command[15];
    char fich[50];
    int fd;
    char *date;
    int key;
} Elemento;

typedef struct Nodo{
    Elemento *elemento;
    struct Nodo *next;
} Nodo;

typedef Nodo *Lista;

void CreateList(Lista L);
void InsertElement(Lista L, Elemento *e);
void RemoveElement(Lista L, Elemento *e);
void RemoveList(Lista L);
Lista Next(Lista L);
Elemento* GetElemento(Lista L);
char* Direction(Lista L);
int Size(Lista L);
char* Command(Elemento* elemento);
char* Date(Lista L);
void PrintList(Lista L);
void PrintMalloc(Lista L);
void PrintMmap(Lista L);
void PrintShared(Lista L);
void PrintElemento(Elemento* e); //precondicion elemento no nulo

#endif //P0_LIST_H

