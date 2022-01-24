/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "lista_historic.h"

void CreateListHistoric(List L){

    L->next = NULL;
}

void InsertElementHistoric(List L, Element *e){

    Node *new = malloc(sizeof(Node));
    new->element = e;
    new->next = NULL;

    if (L->next == NULL){
        L->next = new;
    }else{

        Node *tmp = L->next;
        while (tmp->next != NULL)
            tmp = tmp->next;
        tmp->next = new;
    }
}

void RemoveHistoric(List L){

    if(L->next == NULL)
        return;

    Node *aux = L->next;
    Node *sig;
    while (aux != NULL){
        sig = aux->next;
        free(aux);
        aux = sig;
    }
    L->next = NULL;
}

void PrintListHistoric(List L){

    int cnt=0;
    Node *tmp = L->next;
    while (tmp != NULL){
        strtok(tmp->element->comando, "\n");
        printf("%d->%s\n", cnt, tmp->element->comando);
        tmp = tmp->next;
        cnt++;
    }
}
