/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "lista_procesos.h"

/** Implementación para la lista: Array */

#define TERMINATED_NORMALLY 2
#define TERMINATED_SIGNAL 4
#define STOPPED 8
#define COREDUMP 16
#define CONTINUED 32

/******************************SENALES ******************************************/

struct SEN{
    char *nombre;
    int senal;
};
static struct SEN sigstrnum[]={
        "HUP", SIGHUP,
        "INT", SIGINT,
        "QUIT", SIGQUIT,
        "ILL", SIGILL,
        "TRAP", SIGTRAP,
        "ABRT", SIGABRT,
        "IOT", SIGIOT,
        "BUS", SIGBUS,
        "FPE", SIGFPE,
        "KILL", SIGKILL,
        "USR1", SIGUSR1,
        "SEGV", SIGSEGV,
        "USR2", SIGUSR2,
        "PIPE", SIGPIPE,
        "ALRM", SIGALRM,
        "TERM", SIGTERM,
        "CHLD", SIGCHLD,
        "CONT", SIGCONT,
        "STOP", SIGSTOP,
        "TSTP", SIGTSTP,
        "TTIN", SIGTTIN,
        "TTOU", SIGTTOU,
        "URG", SIGURG,
        "XCPU", SIGXCPU,
        "XFSZ", SIGXFSZ,"VTALRM", SIGVTALRM,
        "PROF", SIGPROF,
        "WINCH", SIGWINCH,
        "IO", SIGIO,
        "SYS", SIGSYS,
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
        "POLL", SIGPOLL,
#endif
#ifdef SIGPWR
        "PWR", SIGPWR,
#endif
#ifdef SIGEMT
        "EMT", SIGEMT,
#endif
#ifdef SIGINFO
        "INFO", SIGINFO,
#endif
#ifdef SIGSTKFLT
        "STKFLT", SIGSTKFLT,
#endif
#ifdef SIGCLD
        "CLD", SIGCLD,
#endif
#ifdef SIGLOST
        "LOST", SIGLOST,
#endif
#ifdef SIGCANCEL
        "CANCEL", SIGCANCEL,
#endif
#ifdef SIGTHAW
        "THAW", SIGTHAW,
#endif
#ifdef SIGFREEZE
        "FREEZE", SIGFREEZE,
#endif
#ifdef SIGLWP
        "LWP", SIGLWP,
#endif
#ifdef SIGWAITING
        "WAITING", SIGWAITING,
#endif
        NULL,-1,};

/********************************************************************************/

void createEmptyProList (elem* array){
    for (int i = 0; i<128; i++){
        strcpy(array[i].commands, "null");
        array[i].pid=(-1);
        array[i].priority=(-1);
    }
}

void addProcess (elem* array, elem pro){

    for (int i = 0; i<128; i++){
        if (strcmp(array[i].commands, "null")==0) {
            array[i]=pro;
            return;
        }
    }
    printf("ArrayList de procesos lleno.");
}

void removeProcess (elem* array, int pos){

    /* Invalid delete position */
    if (pos < 0 || pos > 128){
        printf("Invalid position to remove\n");
    }else {
        /* Copy next element value to current element */
        for (int i = pos - 1; i < 128 - 1; i++) {
            array[i] = array[i + 1];
        }
    }
}

int Senal(char * sen) /*devuelve el numero de senial a partir del nombre*/ {
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}

char *NombreSenal(int sen) /*devuelve el nombre senal a partir de la senal*/{
    /* para sitios donde no hay sig2str*/
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

int causes_process_terminated ( int status, int *cause ){
    if (WIFEXITED(status)){
        *cause = TERMINATED_NORMALLY;
        return WEXITSTATUS(status);
    }

    else if (WIFSIGNALED(status)){
        *cause = TERMINATED_SIGNAL;
        return WTERMSIG(status);
    }

    else if (WIFSTOPPED(status)){
        *cause = STOPPED;
        return WSTOPSIG(status);
    }

    else if (WCONTINUED){
        *cause = CONTINUED;
        return 2<<20;
    }
}

void concat_terminated (int signal, elem* array, int i){
    char buff[50];
    strcpy (buff, "TERMINADO (");
    char str[33];
    sprintf(str, "%d", signal);
    strcat(buff, str);
    strcat(buff, ")");
    strcpy(array[i].state, buff);
}

void concat_terminated_signal (int signal, elem* array, int i){
    char buff[50];
    strcpy (buff, "SIGNALED (");
    strcat(buff, NombreSenal(signal));
    strcat(buff, ")");
    strcpy(array[i].state, buff);
}

void concat_stopped_signal (int signal, elem* array, int i){
    char buff[50];
    strcpy (buff, "STOPPED (");
    strcat(buff, NombreSenal(signal));
    strcat(buff, ")");
    strcpy(array[i].state, buff);
}

void printProcesses (elem* array){
    int i = 0;
    int status, pid, signal, cause;
    while (strcmp(array[i].commands, "null")!=0){

        cause = 0;
        pid = waitpid(array[i].pid, &status, WNOHANG |WUNTRACED |WCONTINUED);

        if (pid == -1 && array[i].state[0] == '\0'){
            perror(" ");
        }else if (pid == -1 ){
            printf("%6d p=%d %s %s %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date, array[i].state, array[i].commands);
        }else if (pid == 0 && array[i].state[0] == '\0'){
            printf("%6d p=%d %s ACTIVE %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date, array[i].commands);
            strcpy(array[i].state, "ACTIVE");
        }else if (pid == 0){
            printf("%6d p=%d %s %s %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date, array[i].state, array[i].commands);

        } else {
            signal = causes_process_terminated (status, &cause);
            printf("%6d p=%d %s ", pid, getpriority( PRIO_PROCESS, array[i].pid), array[i].date);

            if (cause & TERMINATED_NORMALLY){
                printf("TERMINADO (%d) ", signal);
                concat_terminated(signal, array, i);
            }else if (cause & TERMINATED_SIGNAL){
                printf("SIGNALED (%s) ", NombreSenal(signal));
                concat_terminated_signal(signal, array, i);
            }else if ( cause & STOPPED ){
                printf("STOPPED (%s) ", NombreSenal(signal));
                concat_stopped_signal(signal, array, i);
            }else if (cause & CONTINUED){
                printf("ACTIVE ");
                strcpy(array[i].state, "ACTIVE");
            }

            printf(" %s\n", array[i].commands);
        }
        i++;
    }

}

void printProcess (int i, elem* array) {

    int status, pid, signal, cause;

    cause = 0;
    pid = waitpid(array[i].pid, &status, WNOHANG | WUNTRACED | WCONTINUED);

    if (pid == -1 && array[i].state[0] == '\0') {
        perror(" ");
    } else if (pid == -1) {
        printf("%6d p=%d %s %s %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date,
               array[i].state, array[i].commands);
    } else if (pid == 0 && array[i].state[0] == '\0') {
        printf("%6d p=%d %s ACTIVE %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date,
               array[i].commands);
        strcpy(array[i].state, "ACTIVE");
    } else if (pid == 0) {
        printf("%6d p=%d %s %s %s\n", array[i].pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date,
               array[i].state, array[i].commands);

    } else {
        signal = causes_process_terminated(status, &cause);
        printf("%6d p=%d %s ", pid, getpriority(PRIO_PROCESS, array[i].pid), array[i].date);

        if (cause & TERMINATED_NORMALLY) {
            printf("TERMINADO (%d) ", signal);
            concat_terminated(signal, array, i);
        } else if (cause & TERMINATED_SIGNAL) {
            printf("SIGNALED (%s) ", NombreSenal(signal));
            concat_terminated_signal(signal, array, i);
        } else if (cause & STOPPED) {
            printf("STOPPED (%s) ", NombreSenal(signal));
            concat_stopped_signal(signal, array, i);
        } else if (cause & CONTINUED) {
            printf("ACTIVE ");
            strcpy(array[i].state, "ACTIVE");
        }

        printf(" %s\n", array[i].commands);
    }

}