/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/


#ifndef P2_P2FUNCTIONS_H
#define P2_P2FUNCTIONS_H
#include "list.h"

void memoryAllocateMalloc (int tam, Lista L);
void * MmapFichero (char * fichero, int protection, Lista L);
void Cmd_AllocateMmap (char *arg[], Lista L);
void * ObtenerMemoriaShmget (key_t clave, size_t tam, Lista L);
void Cmd_AllocateCreateShared (char *arg[], Lista L);
void memoryAllocateShared (int key, Lista L);

void memoryDeallocMalloc(int tam, Lista memL);
void memoryDeallocMmap(char* fich, Lista memL);
void memoryDeallocShared(int cl, Lista memL);
void memoryDeallocAddr(char* addr, Lista memL);

void show (char *arg, Lista L);
void showAll(Lista L);
void showMalloc(Lista L);
void showMmap(Lista L);
void showShared(Lista L);
void showVars ();
void showProgFuncs ();
void showCFuncs ();

void Cmd_deletekey (char *args[]);
void memdump(int bytes, char* addr);
void memfill(int bytes, char* value, char* addr);
void Cmd_dopmap ();
void * stringToAddr (char *addr);
void doRecursiva (int n);
ssize_t LeerFichero (char *fich, void *p, ssize_t n);
void readFile (char *args[]);
ssize_t EscribirFichero (char *fich, void *p, ssize_t n, char *mod);
void writeFile (char *args[]);

#endif //P2_P2FUNCTIONS_H
