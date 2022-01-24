/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef LAST_SHELL_P3FUNCTIONS_H
#define LAST_SHELL_P3FUNCTIONS_H
#include "list.h"
#include "lista_procesos.h"

char * NombreUsuario (uid_t uid);
uid_t UidUsuario (char * nombre);
void Cmd_getuid ();
void Cmd_setuid (char *tr[]);


void getPriority(int pid);
void setPriority(int pid, int value);
void getUid();
void setUid(char *tr[]);
pid_t forkWait();
void execute(char* name, char* commands[], int pri);
void foreground(char* name, char* commands[], int pri);
void background(char* name, char* commands[], int pri, elem* P);
bool IsValidNumber(char* string);
void proc (char* commands[], elem* P);
void deleteprocsTerm (elem* P);
void deleteprocsSig (elem* P);

#endif //LAST_SHELL_P3FUNCTIONS_H
