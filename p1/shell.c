/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 05/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "historicList.h"
#include "delete.h"
#include "list.h"

#include <ctype.h>

#define MAXCOM 50
#define MAXDIR 500

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
    if (strcmp(subCommand, "-l")==0){
        printf ("joaquin.solla\n");
        printf ("saul.leyva.santaren\n");
    }
    if (strcmp(subCommand, "-n")==0){
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

void procesarEntrada (char* entry, List L, int I);

void historic (char* subCommand, List L, int I) {

    if (strcmp(subCommand, "") == 0) {                                                                                  //historic
        PrintList(L);
    } else {

        if (strcmp(subCommand, "-c") == 0) {																			//historic -c
            Remove(L);
        }

        if (strncmp(subCommand, "-r", 2) ==0) {																			//historic -rN
            char delimitador[] = "-r";
            char *chnum = strtok(subCommand, delimitador);
            int num = atoi(chnum);

            if(num<=I){

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
                procesarEntrada(Rentry, L, I);

            } else {
                printf ("No hay elemento %d en el histórico\n", num);
            }
        }

        if (strncmp(subCommand, "-", 1) == 0 && strcmp(subCommand, "-c\n") != 0 && strncmp(subCommand, "-r", 2) != 0) {					//historic -N

            char *chnum = strtok(subCommand, "-");

            int num = atoi(chnum);
            if (num <= I && isdigit(num)) {
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

void create(char* subCommand, char* name, int ntrozos){
    if(ntrozos == 2 && strcmp(subCommand, "-dir") != 0){
        if(open(subCommand, O_CREAT | O_EXCL, 0777)<0) {
            printf("Imposible crear fichero: %s\n", strerror(errno));
        }
    }
    else if((ntrozos == 1)||(ntrozos == 2 && strcmp(subCommand, "-dir")== 0)) { //se imprime
        struct dirent *de;
        DIR *dr = opendir(".");
        struct stat fileStat;

        if (dr == NULL)  // opendir returns NULL if couldn't open directory
        {
            printf("Could not open current directory\n");
        } else {
            printf("******\n");
            while ((de = readdir(dr)) != NULL) {
                stat(de->d_name, &fileStat);
                printf("%10jd    %s\n", (intmax_t) fileStat.st_size, de->d_name);
            }
            closedir(dr);
        }
    }
    else if(ntrozos == 3 && strcmp(subCommand, "-dir") == 0){
        if(mkdir(name,  0777)<0) {
            printf("Imposible crear fichero: %s\n", strerror(errno));
        }
    }
}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void procesarEntrada(char* entry, List L, int I) {

    bool validCommand = false;

    char *aux[MAXCOM];
    char *commands[MAXCOM];

    strcpy((char *) aux, entry);

    int ntrozos = TrocearCadena((char *) aux, commands);

    if(ntrozos == 1){
        if ((strcmp(commands[0], "authors") == 0)){
            authors("");
            validCommand = true;
        }
        else if ((strcmp(commands[0], "getpid") == 0)){
            getPid();
            validCommand = true;
        }
        else if (strcmp(commands[0], "getppid") == 0) {
            getPPid();
            validCommand = true;
        }
        else if ((strcmp(commands[0], "pwd")) == 0 || (strcmp(commands[0], "chdir") == 0)) {
            Pwd();
            validCommand = true;
        }
        else if (strcmp(commands[0], "date") == 0) {
            date();
            validCommand = true;
        }
        else if (strcmp(commands[0], "time") == 0) {
            Time();
            validCommand = true;
        }
        else if (strcmp(commands[0], "historic") == 0) {
            historic("", L, I);
            validCommand = true;
        } else if(strcmp(commands[0], "create") == 0) {
            create("","", ntrozos);
            validCommand = true;
        } else validCommand = false;
    }

    else if(ntrozos == 2){
        if((strcmp(commands[0], "authors") == 0)){
            authors(commands[1]);
            validCommand = true;
        }
        else if(strcmp(commands[0], "chdir") == 0) {
            Chdir(commands[1]);
            validCommand = true;
        }
        else if(strcmp(commands[0], "historic") == 0) {
            historic(commands[1], L, I);
            validCommand = true;
        }
        else if(strcmp(commands[0], "create") == 0) {
            create(commands[1],"", ntrozos);
            validCommand = true;
        } else validCommand = false;
    }

    else if(ntrozos == 3){
        if(strcmp(commands[0], "create") == 0) {
            create(commands[1],commands[2], ntrozos);
            validCommand = true;
        }
    }

    if(ntrozos >= 1){
        if(strcmp(commands[0], "delete") == 0) {
            delete(commands, ntrozos);
            validCommand = true;
        }
    }

    if(strcmp(commands[0], "list")==0){
        list((char **)commands, ntrozos, (char *) ".", 0);
        validCommand = true;
    }

    if (validCommand == false || (ntrozos == 0)) {
        printf("%s no encontrado\n", commands[0]);
    }

}


int main() {

    //creamos la lista
    List L = malloc(sizeof(Node));
    CreateList(L);
    int I = (0);

    bool terminado = false;
    int totalcommands = (0);
    Element E[MAXCOM];

    while (!terminado) {
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
            procesarEntrada(entry, L, I);
        }
        I++;
    }
    Remove(L);
    free(L);

    return 0;
}