/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef P3_LISTA_PROCESOS_H
#define P3_LISTA_PROCESOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>

typedef struct {
    int pid;
    int priority;
    char* date;
    char commands[128];
    char state[128];
} elem;

void createEmptyProList (elem* array);
void addProcess (elem* array, elem pro);
void removeProcess (elem* array, int pos);
int Senal(char * sen);
char *NombreSenal(int sen);
int causes_process_terminated (int status, int *cause);
void concat_terminated (int signal, elem* array, int i);
void concat_terminated_signal (int signal, elem* array, int i);
void concat_stopped_signal (int signal, elem* array, int i);
void printProcesses (elem* array);
void printProcess (int i, elem* array);

#endif //P3_LISTA_PROCESOS_H