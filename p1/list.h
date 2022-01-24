/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 05/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#ifndef P1_LIST_H
#define P1_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <pwd.h>
#include <grp.h>

#define MAXCOM 50
#define MAXDIR 500

int TrocearCadena(char *, char * []);

char LetraTF (mode_t m);

char * ConvierteModo2 (mode_t m);

void list(char* [], int, char *, int);

#endif //P1_LIST_H