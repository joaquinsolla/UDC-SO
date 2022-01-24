/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef P0_LISTA_HISTORIC_H
#define P0_LISTA_HISTORIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCOM 50
#define MAXDIR 500

typedef struct {
    char comando[MAXCOM];
} Element;

typedef struct Node{
    Element *element;
    struct Node *next;
} Node;

typedef Node *List;

void CreateListHistoric(List L);
void InsertElementHistoric(List L, Element *e);
void RemoveHistoric(List L);
void PrintListHistoric(List L);

#endif //P0_LISTA_HISTORIC_H

