/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 09/10/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#define MAXCOM 50


//	FUNCIONES LISTA:


typedef struct {
    char comando[MAXCOM];
} Element;

typedef struct Node{
    Element *element;
    struct Node *next;
} Node;

typedef Node *List;


void CreateList(List L){

    L->next = NULL;
}

void InsertElement(List L, Element *e){

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

void Remove(List L){

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

void PrintList(List L){

    int cnt=0;
    Node *tmp = L->next;
    while (tmp != NULL){
        strtok(tmp->element->comando, "\n");
        printf("%d->%s\n", cnt, tmp->element->comando);
        tmp = tmp->next;
        cnt++;
    }
}



//	FIN FUNCIONES LISTA


void imprimirPrompt (){
    printf ("->");
}

char* leerEntrada (){
    static char entrada [50];
    fgets (entrada, 50, stdin);
    return entrada;
}

void authors (char* subCommand){
    if (strcmp(subCommand, "")==0){
        printf ("Joaquín Solla Vázquez: joaquin.solla\n");
        printf ("Saúl Leyva Santarén: saul.leyva.santaren\n");
    }
    if (strcmp(subCommand, "-l\n")==0){
        printf ("joaquin.solla\n");
        printf ("saul.leyva.santaren\n");
    }
    if (strcmp(subCommand, "-n\n")==0){
        printf ("Joaquín Solla Vázquez\n");
        printf ("Saúl Leyva Santarén\n");
    }
}

void getPid(){

    pid_t pid = getpid();
    printf("%d\n", pid);

}

void getPPid(){

    pid_t ppid = getppid();
    printf("%d\n", ppid);

}

void Pwd (){

    char dir[200];
    printf("%s\n", getcwd(dir, 200));

}

void Chdir(char* subCommand){

    strtok(subCommand, "\n");
    if(chdir(subCommand)!=0) {
        printf("Imposible cambiar directorio: %s\n", strerror(errno));
    }
}

void date (){

    time_t date = time(NULL);
    struct tm tm = *localtime(&date);
    printf("%02d/%02d/%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

}

void Time(){

    time_t Time = time(NULL);
    struct tm tm = *localtime(&Time);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);

}

void procesarEntrada (char* entry, List L, int totalcommands);

void historic (char* subCommand, List L, int totalcommands) {

    if (strcmp(subCommand, "") ==0) {                                                                                                       //historic

        PrintList(L);


    } else {

        if (strcmp(subCommand, "-c\n") ==
            0) {																															//historic -c

            Remove(L);
        }

        if (strncmp(subCommand, "-r", 2) ==0) {																								//historic -rN
            char delimitador[] = "-r";
            char *chnum = strtok(subCommand, delimitador);
            int num = atoi(chnum);

            if(num<=totalcommands){

                Node *tmp = L->next;
                for (int i = 0; i != num; i++) {
                    //printf("%d->%s\n",i,point->com);
                    tmp = tmp->next;
                }

                char *Rentry = tmp->element->comando;
                //printf ("C: %s\n", Rentry);


                char *a = Rentry;
                strtok(a, "\n");
                if(strcmp(Rentry, a)==0)
                    strcat(Rentry, "\n");
                printf("Ejecutando historic (%d): %s", num, Rentry);
                procesarEntrada(Rentry, L, totalcommands);

            } else {
                printf ("No hay elemento %d en el histórico\n", num);
            }
        }

        if (strncmp(subCommand, "-", 1) == 0 && !(strcmp(subCommand, "-c\n") == 0) &&!(strncmp(subCommand, "-r", 2) == 0)) {					//historic -N

            char *chnum = strtok(subCommand, "-");
            int num = atoi(chnum);
            if (num <= totalcommands) {
                Node *tmp = L->next;
                for (int i = 0; i <= num; i++) {
                    strtok(tmp->element->comando, "\n");
                    printf("%d->%s\n", i, tmp->element->comando);
                    tmp = tmp->next;
                }
            }
        }
    }
}


void procesarEntrada(char *entry, List L, int totalcommands) {

    bool validCommand = false;

    //TROCEO CADENA
    char *mainCommand;
    char *subCommand;
    char delimitador[] = " ";

    char a[MAXCOM];
    strcpy(a, entry);
    char *trozo = strtok(a, delimitador);

    if (trozo != NULL) {

        mainCommand = trozo;
        //printf("Main: %s\n", mainCommand);
        trozo = strtok(NULL, delimitador);

        if (trozo != NULL) {
            subCommand = trozo;
            //printf("Sub: %s\n", subCommand);
        } else { subCommand = ""; }
    }
    //END TROCEO CADENA


    if ((strcmp(mainCommand, "authors") == 0) || (strcmp(mainCommand, "authors\n") == 0)) {

        authors(subCommand);
        validCommand = true;
    }

    if (strcmp(mainCommand, "getpid\n") == 0) {

        getPid();
        validCommand = true;
    }

    if (strcmp(mainCommand, "getppid\n") == 0) {

        getPPid();
        validCommand = true;
    }

    if ((strcmp(mainCommand, "pwd\n")) == 0 || (strcmp(mainCommand, "chdir\n") == 0)) {

        Pwd();
        validCommand = true;
    }

    if (strcmp(mainCommand, "chdir") == 0) {

        Chdir(subCommand);
        validCommand = true;
    }

    if (strcmp(mainCommand, "date\n") == 0) {

        date();
        validCommand = true;
    }

    if (strcmp(mainCommand, "time\n") == 0) {

        Time();
        validCommand = true;
    }

    if ((strcmp(mainCommand, "historic") == 0) || (strcmp(mainCommand, "historic\n") == 0)) {

        historic(subCommand, L, totalcommands);
        validCommand = true;
    }


    if (validCommand == false && !(strcmp(mainCommand, "\n") == 0)) {

        strtok(mainCommand, "\n");
        printf("%s no encontrado\n", mainCommand);
    }

}

int main() {

    //creamos la lista
    List L = malloc(sizeof(Node));
    CreateList(L);
    int I = (-1);

    bool terminado = false;
    int totalcommands = (0);
    Element E[MAXCOM];

    while (!terminado) {

        I++;

        char *entry;

        imprimirPrompt();
        entry = leerEntrada();
        totalcommands++;

        //insertamos el comando en la lista
        strcpy(E[I].comando, entry);
        //printf("E.comando: %s\n", E[I].comando);
        InsertElement(L, &E[I]);

        if ((strcmp(entry, "quit\n") == 0) || (strcmp(entry, "end\n") == 0) || (strcmp(entry, "exit\n") == 0)) {
            terminado = true;
        } else {
            procesarEntrada(entry, L, totalcommands);
        }
    }
    Remove(L);
    free(L);

    return 0;
}
