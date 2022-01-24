/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include <sys/resource.h>
#include <unistd.h>
#include <pwd.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include "p3functions.h"
#include "lista_procesos.h"


/******************************Otras funciones del boletín******************************/

char * NombreUsuario (uid_t uid){
    struct passwd *p;if ((p=getpwuid(uid))==NULL)
        return (" ??????");
    return p->pw_name;
}

uid_t UidUsuario (char * nombre){
    struct passwd *p;
    if ((p=getpwnam (nombre))==NULL)
        return (uid_t) -1;
    return p->pw_uid;
}

void Cmd_getuid (){
    uid_t real=getuid(), efec=geteuid();
    printf ("Credencial real: %d, (%s)\n", real, NombreUsuario (real));
    printf ("Credencial efectiva: %d, (%s)\n", efec, NombreUsuario (efec));
}

void Cmd_setuid (char *tr[]){
    uid_t uid;
    int u;
    if (tr[0]==NULL || (!strcmp(tr[0],"-l") && tr[1]==NULL)){
        Cmd_getuid();
        return;
    }
    if (!strcmp(tr[0],"-l")){
        if ((uid=UidUsuario(tr[1]))==(uid_t) -1){
            printf ("Usuario no existente %s\n", tr[1]);
            return;
        }
    }
    else if ((uid=(uid_t) ((u=atoi (tr[0]))<0)? -1: u) ==(uid_t) -1){
        printf ("Valor no valido de la credencial %s\n",tr[0]);
        return;
    }
    if (setuid (uid)==-1)
        printf ("Imposible cambiar credencial: %s\n", strerror(errno));
}



/******************************Funciones que hay que implementar******************************/

void getPriority(int pid){
    /*pid = 0 is the actual process*/
    if(pid != 0) printf("Prioridad del proceso %d es %d\n", pid, getpriority(PRIO_PROCESS, pid));
    else printf("Prioridad del proceso %d es %d\n", getpid(), getpriority(PRIO_PROCESS, pid));
}

void setPriority(int pid, int value){
    if(setpriority(PRIO_PROCESS, pid, value) != 0) perror("No se pudo cambiar la prioridad\n");
}

void getUid(){
    Cmd_getuid();
}

void setUid(char *tr[]){
    Cmd_setuid((char **) tr);
}

pid_t forkWait(){
    pid_t pid;

    pid = fork();
    if (pid) waitpid(pid, NULL, 0);
    return pid;
}

void execute(char* name, char* commands[], int pri){
    if(pri != -21) setPriority(0,pri);
    if(execvp(name, commands) == -1) perror ("Cannot execute");
    exit(255);
}

void foreground(char* name, char* commands[], int pri) {
    pid_t pid = fork();

    if (pid==0){ //Proceso hijo
        if(pri != -21) setPriority(pid,pri);
        if(execvp(name, commands) == -1) perror ("Cannot execute");
        exit(255);
    }
    waitpid (pid,NULL,0);
}

void background(char* name, char* commands[], int pri, elem* P){
    pid_t pid = fork();

    //INSERTAR ELEM EN LISTA
    elem E;
    time_t CurrentTime;
    time(&CurrentTime);

    E.pid = pid;
    E.date = ctime(&CurrentTime);
    E.date[strlen(E.date)-1] = '\0';
    E.state[0] = '\0';
    strcpy(E.commands, commands[0]);
    int i = 1;
    while (commands[i] != (void *)0){
        strcat(E.commands, " ");
        strcat(E.commands, commands[i]);
        i++;
    }

    addProcess(P, E);

    if (pid==0){ //Proceso hijo
        if(pri != -21) setPriority(pid,pri);
        if(execvp(name, commands) == -1) perror ("Cannot execute");

        int p = -1;
        for (int j = 0; i < (sizeof P); j++) {
            if (P[j].pid == E.pid){
                p = j;
            }
        }

        removeProcess(P, p);
        exit(255);
    }
    //does not wait
}

bool IsValidNumber(char* string) {
    for (int i = 0; i < strlen( string ); i ++){
        //ASCII value of 0 = 48, 9 = 57. So if value is outside of numeric range then fail
        //Checking for negative sign "-" could be added: ASCII value 45.
        if (string[i] < 48 || string[i] > 57) return false;
    }
    return true;
}

void proc (char* commands[], elem* P){

    if (IsValidNumber(commands[1])){
        int p = -1;
        for (int i = 0; i < (sizeof P); i++) {
            if (P[i].pid == atoi(commands[1])){
                p = i;
            }
        }
        if (p==(-1)) printProcesses(P);
        else printProcess(p, P);

    } else if (strcmp(commands[1], "-fg")==0 && IsValidNumber(commands[2])){
        int p = -1;
        for (int i = 0; i < (sizeof P); i++) {
            if (P[i].pid == atoi(commands[2])){
                p = i;
            }
        }
        if (p==(-1)) printf("Proceso %d pid ya esta finalizado\n", atoi(commands[2]));

        else if (strncmp(P[p].state, "TERMINADO", 8)==0){
            printf("Proceso %d pid ya esta finalizado\n", atoi(commands[2]));

        } else {
            waitpid (atoi(commands[2]),NULL,0);
            printProcess(p, P);
            removeProcess(P, p);
        }

    } else printf("Invalid entry.\n");

}

void deleteprocsTerm (elem* P){

    for (int i = 0; i < 128; i++) {

        int len = strlen(P[i].state);
        const char* last3 = &P[i].state[len-3];

        if (strcmp(last3, "(0)")==0){
            removeProcess(P, i);
            i-=1;
        }
    }
}

void deleteprocsSig (elem* P){

    for (int i = 0; i < 128; i++) {

        int len = strlen(P[i].state);
        const char* last3 = &P[i].state[len-3];
        const char* last = &P[i].state[len-1];

        if (strcmp(last3, "(0)")!=0 && strcmp(last, ")")==0){
            removeProcess(P, i);
            i-=1;
        }
    }
}