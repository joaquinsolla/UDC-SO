/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 05/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef P1_HISTORICLIST_H
#define P1_HISTORICLIST_H

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

void CreateList(List L);
void InsertElement(List L, Element *e);
void Remove(List L);
void PrintList(List L);

#endif //P1_HISTORICLIST_H