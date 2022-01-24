/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include "list.h"
#define LEERCOMPLETO ((ssize_t)-1)
#define TAMANO 4096

/**ALLOCATE*/

void memoryAllocateMalloc (int tam, Lista L){
    time_t CurrentTime;
    time(&CurrentTime);
    char *ptr = (char *) malloc(tam);
    if (ptr == NULL) {
        printf("Error\n");
    } else {
        Elemento *e =  malloc(sizeof(Elemento));
        e->dir = ptr;
        e->size = tam;
        strcpy(e->command, "malloc");
        e->date = ctime(&CurrentTime);
        printf("allocated %d at %p\n", tam, e->dir);
        InsertElement(L, e);
    }
    free(ptr);
}

void * MmapFichero (char * fichero, int protection, Lista L){
    time_t CurrentTime;
    time(&CurrentTime);
    Elemento *e = malloc(sizeof(Elemento));

    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;

    e->dir = p;
    e->size = s.st_size;
    e->date = ctime(&CurrentTime);
    strcpy(e->command, "mmap");
    strcpy(e->fich, fichero);
    e->fd = df;

    InsertElement(L, e);
    return p;
}

void Cmd_AllocateMmap (char *arg[], Lista L) {
    char *perm;
    void *p;
    int protection=0;
    if (arg[0]==NULL)
    {PrintMmap(L); return;}
    if ((perm=arg[1])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MmapFichero(arg[0],protection, L))==NULL)
        perror ("Imposible mapear fichero");
    else
        printf ("fichero %s mapeado en %p\n", arg[0], p);
}

void * ObtenerMemoriaShmget (key_t clave, size_t tam, Lista L){

    time_t CurrentTime;
    time(&CurrentTime);
    Elemento *e = malloc(sizeof(Elemento));

    void * p;
    int aux=0,id=0,flags=0777;
    struct shmid_ds s;
    if (tam)                                    //si tam no es 0 la crea en modo exclusivo
        flags=flags | IPC_CREAT | IPC_EXCL;     //si tam es 0 intenta acceder a una ya creada

    if (clave==IPC_PRIVATE){                    //no nos vale
        errno=EINVAL;
        return NULL;
    }
    id=shmget(clave, tam, flags);
    if (id==-1)
        return (NULL);

    p=shmat(id,NULL,0);
    if (p==(void*) -1){
        aux=errno;                              //si se ha creado y no se puede mapear
        if (tam)                                //se borra
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    // Guardar En Direcciones de Memoria Shared (p, s.shm_segsz, clave.....):

    e->dir = p;
    e->size = (int)s.shm_segsz;
    strcpy(e->command, "shared memory");
    e->key = (int) clave;
    e->date = ctime(&CurrentTime);

    InsertElement(L, e);

    return (p);
}

void Cmd_AllocateCreateShared (char *arg[], Lista L){
    //arg[0] is the key and arg[1] is the size
    key_t k;
    size_t tam=0;
    void *p;
    if (arg[0]==NULL || arg[1]==NULL)
    {PrintShared(L);
        return;}
    k=(key_t) atoi(arg[0]);
    if (arg[1]!=NULL)
        tam=(size_t) atoll(arg[1]);
    if ((p=ObtenerMemoriaShmget(k,tam, L))==NULL)
        perror ("Imposible obtener memoria shmget");
    else
        printf ("Allocated shared memory (key %d) at %p\n",k,p);
}

void memoryAllocateShared ( int key, Lista L){
    void *p;
    key_t k;
    k=(key_t) key;

    if ((p=ObtenerMemoriaShmget(k, 0, L)) == NULL ) perror("Cannot allocate");
    else printf ("Allocated shared memory (key %d) at %p\n",k,p);
}


/**DEALLOC*/

void memoryDeallocMalloc(int tam, Lista memL) {
    int cnt=0;

    Lista aux = memL->next;

    while (aux != NULL) {
        if (tam == Size(aux) && strcmp(Command(GetElemento(aux)), "malloc") == 0) {
            printf("block at address %p deallocated (malloc)\n", (char *) Direction(aux));
            RemoveElement(memL,GetElemento(aux));
            cnt++;
            break;
        }
        aux = Next(aux);
    }

    if(cnt == 0) PrintMalloc(memL);
}

void memoryDeallocMmap(char* fich, Lista memL) {
    int cnt=0, fail=0;

    Lista aux = memL->next;

    while (aux != NULL) {
        if (strcmp(aux->elemento->fich, fich) == 0 && strcmp(Command(GetElemento(aux)), "mmap") == 0) {
            fail = close(aux->elemento->fd);
            if (fail != 0) {
                perror("The file does not close\n");
                break;
            }
            fail = munmap( Direction(aux), Size(aux));
            if (fail != 0) {
                perror("Unmap not possible\n");
                break;
            }
            printf("block at address %p deallocated (mmap)\n", (char *) Direction(aux));
            RemoveElement(memL, GetElemento(aux));
            cnt++;
            break;
        }
        aux = Next(aux);
    }

    if(cnt == 0) PrintMmap(memL);
}

void memoryDeallocShared(int cl, Lista memL) {
    int cnt=0, fail=0;

    Lista aux = memL->next;

    while (aux != NULL) {
        if (aux->elemento->key == cl && strcmp(Command(GetElemento(aux)), "shared memory") == 0) {
            fail =shmdt(Direction(aux));
            if (fail != 0) {
                perror("Not detached\n");
                break;
            }
            printf("block at address %p deallocated (shared)\n", (char *) Direction(aux));
            RemoveElement(memL, GetElemento(aux));
            cnt++;
            break;
        }
        aux = Next(aux);
    }

    if(cnt == 0) PrintShared(memL);
}

void memoryDeallocAddr(char* addr, Lista memL) {
    int cnt=0;
    char *auxc = malloc(sizeof(Direction(Next(memL))));

    Lista aux = memL->next;
//memory -allocate -malloc 1
    while (aux != NULL) {
        snprintf(auxc, 2*sizeof(Direction(aux)), "%p", Direction(aux));
        if (strcmp(addr, auxc) == 0) {
            printf("block at address %p deallocated (%s)\n", Direction(aux), Command(GetElemento(aux)));
            RemoveElement(memL,GetElemento(aux));
            cnt++;
            break;
        }
        aux = Next(aux);
    }

    free(auxc);
    if(cnt == 0) PrintList(memL);
}


/**SHOW*/

int g1 = 0, g2 = 1, g3 = 2;     //Global vars para showvars

void showAll(Lista L){
    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "malloc")==0) printf("\t%p %16d %.13s %s\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date,  tmp->elemento->command);
        if(strncmp(tmp->elemento->command, "mmap", 4)==0) printf("\t%p %16d %.13s %s (descriptor %d)\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date, tmp->elemento->fich, tmp->elemento->fd);
        if(strcmp(tmp->elemento->command, "shared memory")==0) printf("\t%p %16d %.13s shared (key %d)\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date, tmp->elemento->key);
        tmp = tmp->next;
    }
}

void showMalloc(Lista L){

    Nodo *tmp = L->next;

    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "malloc")==0) printf("\t%p %16d %.13s %s\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date,  tmp->elemento->command);
        tmp = tmp->next;
    }
}

void showMmap(Lista L){

    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strncmp(tmp->elemento->command, "mmap", 4)==0) printf("\t%p %16d %.13s %s (descriptor %d)\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date, tmp->elemento->fich, tmp->elemento->fd);
        tmp = tmp->next;
    }
}

void showShared(Lista L){

    Nodo *tmp = L->next;
    while (tmp != NULL){
        if(strcmp(tmp->elemento->command, "shared memory")==0) printf("\t%p %16d %.13s shared (key %d)\n", tmp->elemento->dir, tmp->elemento->size, tmp->elemento->date, tmp->elemento->key);
        tmp = tmp->next;
    }
}

void show (char *arg, Lista L){

    if (strcmp(arg, "-all")==0) showAll(L);
    else if (strcmp(arg, "-malloc")==0) showMalloc(L);
    else if (strcmp(arg, "-mmap")==0) showMmap(L);
    else if (strcmp(arg, "-shared")==0) showShared(L);
}

void showVars (){
    int l1 = 3, l2 = 4, l3 = 5;
    extern int g1, g2, g3;

    printf("Variables locales\t%p,%18p,%18p\n", &l1, &l2, &l3);
    printf("Variables globales\t%p,%18p,%18p\n", &g1, &g2, &g3);
}

void showProgFuncs (){
    printf("Funciones programa\t%p,%18p,%18p\n", &showAll, &showVars, &showMalloc);

}

void showCFuncs (){
    printf("Funciones libreria\t%p,%18p,%18p\n", &atoi, &printf, &strcmp);

}


/**OTHERS*/

void Cmd_deletekey (char *args[]) {
    //arg[0] points to a str containing the key
    key_t clave;
    int id=0;
    char *key=args[0];
    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("rmkey clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        perror ("shmctl: imposible eliminar memoria compartida\n");
}

void * stringToAddr (char *addr){
    void *p;
    sscanf(addr, "%p", &p);
    return p;
}

void memdump(int bytes, char* addr){
    int cnt=0, iter = 0;
    char *auxc = stringToAddr(addr);
    char output[1500];

    for (int i = 0; i < bytes; i++){
        if ( isprint(*auxc) == 0 ) output[i] = ' ';
        else output[i] = *auxc;
        auxc++;
    }

    for(int j = 0; j<bytes; j++){
        printf(" %c  ", output[j]); cnt++;
        if((cnt == 25) || ((cnt%25)+iter*25 == bytes)){
            printf("\n");
            for(int k = j+1-cnt; k<=j; k++)
                printf("%2x  ", output[k]);
            printf("\n");
            cnt = cnt-25;
            iter++;
        }
    }

}

void memfill(int bytes, char* value, char* addr){
    long longValue = strtol(value, NULL, 16);
    int intValue = (int) longValue;
    char *auxc = stringToAddr(addr);

    for (int i = 0; i < bytes; i++){
        *auxc = (char ) intValue;
        auxc++;
    }
}

void Cmd_dopmap () {
    //no arguments necessary
    pid_t pid;
    char elpid[32];
    char *argv[3] = {"pmap", elpid, NULL};
    sprintf(elpid, "%d", (int) getpid());
    if ((pid = fork()) == -1) {
        perror("Imposible crear proceso");
        return;
    }
    if (pid == 0) {
        if (execvp(argv[0], argv) == -1)
            perror("cannot execute pmap");
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

void doRecursiva (int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];
    printf("parametro n:%d en %p\n", n, &n);
    printf("array estatico en:%p \n", estatico);
    printf("array automatico en %p\n", automatico);
    n--;
    if (n > 0)
        doRecursiva(n);
}

ssize_t LeerFichero (char *fich, void *p, ssize_t n){
    //n=-1 indica que se lea todo
    ssize_t nleidos=0,tam=n;
    int df, aux;struct stat s;
    if (stat (fich,&s)==-1 || (df=open(fich,O_RDONLY))==-1)
        return ((ssize_t)-1);
    if (n==LEERCOMPLETO)
        tam=(ssize_t) s.st_size;
    if ((nleidos=read(df,p, tam))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return ((ssize_t)-1);
    }
    close (df);
    return (nleidos);
}

void readFile (char *args[]){
    void * addr = stringToAddr(args[1]);
    int size=0, bytes=0;

    if (args[2] == NULL) {
        bytes = LeerFichero(args[0],args[1], -1);
        printf("leidos %d bytes de %s en %p\n", bytes, args[0], addr);
    }
    else {
        size = atoi(args[2]);
        bytes = LeerFichero(args[0], addr, size);
    }
    printf("leidos %d bytes de %s en %p\n", bytes, args[0], addr);
}

ssize_t EscribirFichero (char *fich, void *p, ssize_t n, char *mod){

    ssize_t  nwrites=0, tam = n;
    int df=0, aux=0;
    struct stat s;

    if ((mod == NULL) && (access(fich, F_OK) != -1) ) {                         //Si fich ya existe y NO se usa -o
        printf("Imposible escribir fichero: File exists\n");
        return ((ssize_t) -1);
    } else {

        df = open(fich, (O_WRONLY | O_RDONLY | O_CREAT));

        if (df == -1 || (stat(fich, &s) == -1)) {
            printf("escritos 0 bytes en %s desde %p\n", fich, p);
            return ((ssize_t)-1);
        }

        nwrites = write(df, p, tam);

        if (nwrites == -1) {
            printf("escritos 0 bytes en %s desde %p\n", fich, p);
            aux=errno;
            close(df);
            errno=aux;
            return ((ssize_t) -1);
        }
    }
    return nwrites;
}

void writeFile (char *args[]) {
    void *addr = stringToAddr(args[1]);
    int size=0, bytes=0;

    size = atoi(args[2]);
    bytes = EscribirFichero(args[1], addr, size, args[3]);

    if (bytes != -1) {
        rename(args[1], args[0]);
        printf("escritos %d bytes en %s desde %p\n", bytes,args[0], args[1]);
    }


}