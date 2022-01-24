/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "list.h"

void CreateList(Lista L){

    L->next = NULL;
}

void InsertElement(Lista L, Elemento *e){

    Nodo *new = malloc(sizeof(Nodo));
    new->elemento = e;
    new->next = NULL;

    if (L->next == NULL){
        L->next = new;
    }else{

        Nodo *tmp = L->next;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new;
    }

}

void RemoveElement(Lista L, Elemento *e) {

    struct Nodo* temp = L->next, *prev;

    if (temp != NULL && temp->elemento == e)
    {
        L->next = temp->next;
        free(temp->elemento);
        free(temp);
        return;
    }

    while (temp != NULL && temp->elemento != e)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;

    free(temp->elemento);
    free(temp);
}

void RemoveList(Lista L){

    if(L->next == NULL)
        return;

    Nodo *aux = L->next;
    Nodo *sig;
    while (aux != NULL){
        sig = aux->next;
        RemoveElement(L, aux->elemento);
        aux = sig;
    }
    L->next = NULL;
}

Lista Next(Lista L){
    return L->next;
}

Elemento* GetElemento(Lista L){
    return L->elemento;
}

char* Direction(Lista L){
    return L->elemento->dir;
}

int Size(Lista L){
    return L->elemento->size;
}

char* Command(Elemento* elemento){
    return elemento->command;
}

char* Date(Lista L){
    return L->elemento->date;
}

void PrintList(Lista L){
    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "malloc")==0) printf("%p: size:%d. %s %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->date);
        if(strncmp(tmp->elemento->command, "mmap", 4)==0) printf("%p: size:%d. %s %s (fd:%d) %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->fich, tmp->elemento->fd, tmp->elemento->date);
        if(strcmp(tmp->elemento->command, "shared memory")==0) printf("%p: size:%d. %s (key %d) %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->key, tmp->elemento->date);
        tmp = tmp->next;
    }
}

void PrintMalloc(Lista L){

    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "malloc")==0) printf("%p: size:%d. %s %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->date);
        tmp = tmp->next;
    }
}

void PrintMmap(Lista L){

    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strncmp(tmp->elemento->command, "mmap", 4)==0) printf("%p: size:%d. %s %s (fd:%d) %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->fich, tmp->elemento->fd, tmp->elemento->date);
        tmp = tmp->next;
    }
}

void PrintShared(Lista L){

    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "shared memory")==0) printf("%p: size:%d. %s (key %d) %s", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->command, tmp->elemento->key, tmp->elemento->date);
        tmp = tmp->next;
    }
}

void PrintElemento(Elemento* e){
    printf("%p: size:%d. %s %s\n", e->dir, e->size, e->command, e->date);
}

