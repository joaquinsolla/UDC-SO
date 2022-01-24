/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN LOGIN 2: saul.leyva.santaren
* GROUP: 2.5
* DATE: 20/11/2020
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
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <pwd.h>
#include <grp.h>

#include "lista_historic.h"
#include "list.h"
#include "p2functions.h"

void procesarEntrada (char* entry, List L, Lista memL, int I);

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

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

void historic (char* subCommand, List L, Lista memL, int I) {

    if (strcmp(subCommand, "") == 0) {                                                                                  //historic
        PrintListHistoric(L);
    } else {

        if (strcmp(subCommand, "-c") == 0) {																			//historic -c
            RemoveHistoric(L);
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
                procesarEntrada(Rentry, L, memL, I);

            } else {
                printf ("No hay elemento %d en el histórico\n", num);
            }
        }

        if (strncmp(subCommand, "-", 1) == 0 && !(strcmp(subCommand, "-c\n") == 0) &&!(strncmp(subCommand, "-r", 2) == 0)) {					//historic -N

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
            printf("Could not open current directory");
        } else {
            printf("****************\n");
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

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int remove_directory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;
        r = 0;

        while (!r && (p=readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode))
                        r2 = remove_directory(buf);
                    else
                        r2 = unlink(buf);
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    } else {
        printf("Imposible Borrar: %s\n", strerror(errno));

    }

    if (!r)
        r = rmdir(path);

    return r;
}

void delete (char** subCommand, int ntrozos) {

    if (ntrozos == 1 && strcmp(subCommand[0], "delete")==0) {                                                           //SHOW FILES

        DIR *d;
        int sz;
        struct dirent *dir;
        d = opendir(".");
        printf("*** .\n");

        if (d) {
            while ((dir = readdir(d)) != NULL) {
                FILE *fp;
                fp = fopen( dir->d_name, "rw");
                fseek(fp, 0L, SEEK_END);
                sz = ftell(fp);

                printf("%8d  %s\n", sz, dir->d_name);
            }
            closedir(d);
        }

    } else {

        if (strcmp(subCommand[1], "-rec")==0){                                                                          //DELETE RECURSIVO

            if (ntrozos == 2) {

                DIR *d;
                int sz;
                struct dirent *dir;
                d = opendir(".");
                printf("*** .\n");

                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        FILE *fp;
                        fp = fopen( dir->d_name, "rw");
                        fseek(fp, 0L, SEEK_END);
                        sz = ftell(fp);

                        printf("%8d  %s\n", sz, dir->d_name);
                    }
                    closedir(d);
                }
            } else {

                for (int i = 2; i < ntrozos; i++) {

                    if (is_regular_file(subCommand[i])){
                        remove(subCommand[i]);
                    }else remove_directory(subCommand[i]);

                }
            }

        } else {                                                                                                        //DELETE SIMPLE

            for (int i = 1; i < ntrozos; i++) {

                if (remove(subCommand[i]) == 0){
                } else printf("Imposible Borrar: %s\n", strerror(errno));

            }
        }

    }

}

char LetraTF (mode_t m){
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK:  return 's';  /*socket */
        case S_IFLNK:   return 'l';    /*symbolic link*/
        case S_IFREG:   return '-';   /* fichero normal*/
        case S_IFBLK:   return 'b';   /*block device*/
        case S_IFDIR:   return 'd';   /*directorio */
        case S_IFCHR:   return 'c';   /*char  device*/
        case S_IFIFO:   return 'p';   /*pipe*/
        default: return '?';   /*desconocido, no deberia aparecer*/}
}

char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';  /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';   /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';   /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';  /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return (permisos);
}

void list(char* commands[MAXCOM], int ntrozos, char* current_directory, int counter_rec) {
    bool longcnt = false, dircnt = false, hidcnt = false, reccnt = false, namecnt = false;
    struct dirent *de;
    DIR *dr = opendir(current_directory);
    char aux_directory[MAXDIR];
    char aux_directory2[MAXDIR];
    struct stat fileStat;
    char *fecha[MAXCOM];
    char *aux[MAXCOM];
    int not_found = 0;

    if (ntrozos != 1) {
        for (int i = 1; i < ntrozos; i++) {
            if (strcmp(commands[i], "-long") == 0)
                longcnt=true;
            else if (strcmp(commands[i], "-dir") == 0)
                dircnt=true;
            else if (strcmp(commands[i], "-hid") == 0)
                hidcnt=true;
            else if (strcmp(commands[i], "-rec") == 0)
                reccnt=true;
            else namecnt=true;
        }
    }

    if (!namecnt) {                                                                                                     //// NO HAY NOMBRES//// el -dir no afecta

        char *comodin[MAXDIR];
        if (dr ==NULL){                                                                                                  //opendir returns NULL if couldn't open directory
            printf("Could not open current directory\n");
        } else {
            if (reccnt) {                                                                                               //tiene -rec
                if (counter_rec == 0) {
                    counter_rec = 0;
                    printf("********* %s\n", current_directory);
                } else {
                    counter_rec = 1;
                    strcpy((char *) comodin, &current_directory[2]);
                    printf("********* %s\n", (char *) comodin);
                }
            }

            while ((de = readdir(dr)) != NULL) {
                if (!hidcnt && strncmp(de->d_name, ".", 1) == 0)
                    continue;                              //no se muestran los que empiezan por '.'

                strcpy(aux_directory, current_directory);
                if (reccnt) {
                    strcat(aux_directory, "/");
                    strcat(aux_directory, de->d_name);
                }

                stat(aux_directory, &fileStat);

                strcpy((char *) aux, ctime((const time_t *) &fileStat.st_mtim));
                TrocearCadena((char *) aux, fecha);

                struct passwd *nameid;
                nameid = getpwuid(fileStat.st_uid);
                struct group *grp;
                grp = getgrgid(fileStat.st_gid);

                if (!longcnt)
                    printf("%10jd    %s\n", (intmax_t) fileStat.st_size, de->d_name);
                else {
                    if (de->d_type == DT_LNK) {
                        char *buf;
                        ssize_t bufsize, nbytes;
                        bufsize = fileStat.st_size + 1;
                        if (fileStat.st_size == 0)
                            bufsize = MAXDIR;
                        buf = malloc(bufsize);
                        if (buf == NULL) {
                            perror("malloc");
                            exit(EXIT_FAILURE);
                        }
                        nbytes = readlink(de->d_name, buf, bufsize);

                        printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s -> %.*s\n", fecha[0],
                               fecha[1], fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name, grp->gr_name,
                               ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
                               (uintmax_t) fileStat.st_nlink, de->d_name, (int) nbytes, buf);
                        free(buf);
                    } else
                        printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s\n", fecha[0], fecha[1],
                               fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name, grp->gr_name,
                               ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
                               (uintmax_t) fileStat.st_nlink, de->d_name);

                }
            }

            if (reccnt) {
                closedir(dr);
                dr = opendir(current_directory);
                while ((de = readdir(dr)) != NULL) {
                    if (strncmp(de->d_name, ".", 1) == 0);
                    else if (de->d_type == DT_DIR) {
                        strcpy(aux_directory, current_directory);
                        strcat(aux_directory, "/");
                        strcat(aux_directory, de->d_name);
                        list(commands, ntrozos, aux_directory, counter_rec);
                    }
                }
            }
        }
    }

    else {                                                                                                              ////HAY NOMBRES////


        if (dr ==
            NULL) {                                                                                         // opendir returns NULL if couldn't open directory
            printf("Could not open current directory\n");
        } else {
            int i;
            if (dircnt) {
                if (reccnt && dircnt && !hidcnt && !longcnt) i = 3;
                else if (reccnt && dircnt && !hidcnt && longcnt) i = 4;
                else if (reccnt && dircnt && hidcnt && !longcnt) i = 4;
                else if (reccnt && dircnt && hidcnt && longcnt) i = 5;
                else if (!reccnt && dircnt && !hidcnt && longcnt) i = 3;
                else if (!reccnt && dircnt && hidcnt && !longcnt) i = 3;
                else if (!reccnt && dircnt && hidcnt && longcnt) i = 4;
                else if (!reccnt && dircnt && !hidcnt && !longcnt) i = 2;

                for (; i < ntrozos; i++) {
                    int y = i;
                    for (; y < ntrozos; y++) {
                        while ((de = readdir(dr)) != NULL) {
                            strcpy(aux_directory, current_directory);
                            strcat(aux_directory, "/");
                            strcat(aux_directory, de->d_name);
                            stat(aux_directory, &fileStat);

                            if (strcmp(de->d_name, commands[i]) == 0) {
                                not_found++;
                            }
                        }
                    }
                    if (not_found == 0)
                        printf("****error al acceder a %s: No such file or directory\n", commands[i]);
                    closedir(dr);
                    dr = opendir(current_directory);

                    if (not_found != 0) {
                        printf("********* %s\n", commands[i]);
                        while ((de = readdir(dr)) != NULL) {
                            strcpy(aux_directory, current_directory);
                            strcat(aux_directory, "/");
                            strcat(aux_directory, de->d_name);
                            stat(aux_directory, &fileStat);

                            if (strcmp(de->d_name, commands[i]) == 0) {
                                struct dirent *de2;
                                DIR *dr2 = opendir(aux_directory);
                                if (dr2 == NULL)
                                    printf("Could not open current directory\n");
                                else {
                                    if (!longcnt) {
                                        while ((de2 = readdir(dr2)) != NULL) {
                                            stat(de2->d_name, &fileStat);
                                            if (!hidcnt && strncmp(de2->d_name, ".", 1) == 0)
                                                continue;
                                            printf("%10jd    %s\n", (intmax_t) fileStat.st_size, de2->d_name);
                                        }
                                    } else {
                                        while ((de2 = readdir(dr2)) != NULL) {
                                            stat(de2->d_name, &fileStat);
                                            if (!hidcnt && strncmp(de2->d_name, ".", 1) == 0)
                                                continue;

                                            strcpy((char *) aux, ctime((const time_t *) &fileStat.st_mtim));
                                            TrocearCadena((char *) aux, fecha);

                                            struct passwd *nameid;
                                            nameid = getpwuid(fileStat.st_uid);
                                            struct group *grp;
                                            grp = getgrgid(fileStat.st_gid);


                                            if (de2->d_type == DT_LNK) {
                                                char *buf;
                                                ssize_t bufsize, nbytes;
                                                bufsize = fileStat.st_size + 1;
                                                if (fileStat.st_size == 0)
                                                    bufsize = MAXDIR;
                                                buf = malloc(bufsize);
                                                if (buf == NULL) {
                                                    perror("malloc");
                                                    exit(EXIT_FAILURE);
                                                }
                                                nbytes = readlink(de2->d_name, buf, bufsize);

                                                printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s -> %.*s\n",
                                                       fecha[0], fecha[1],
                                                       fecha[2], fecha[4], (uintmax_t) fileStat.st_ino,
                                                       nameid->pw_name,
                                                       grp->gr_name,
                                                       ConvierteModo2(fileStat.st_mode),
                                                       (intmax_t) fileStat.st_size,
                                                       (uintmax_t) fileStat.st_nlink, de2->d_name, (int) nbytes,
                                                       buf);
                                                free(buf);
                                            } else
                                                printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s\n",
                                                       fecha[0],
                                                       fecha[1],
                                                       fecha[2], fecha[4], (uintmax_t) fileStat.st_ino,
                                                       nameid->pw_name,
                                                       grp->gr_name,
                                                       ConvierteModo2(fileStat.st_mode),
                                                       (intmax_t) fileStat.st_size,
                                                       (uintmax_t) fileStat.st_nlink, de2->d_name);
                                        }
                                    }
                                    closedir(dr2);
                                    if (reccnt) {
                                        dr2 = opendir(aux_directory);
                                        while ((de2 = readdir(dr2)) != NULL) {
                                            if (strncmp(de2->d_name, ".", 1) == 0);
                                            else if (de2->d_type == DT_DIR) {
                                                strcpy(aux_directory2, aux_directory);
                                                strcat(aux_directory2, "/");
                                                strcat(aux_directory2, de2->d_name);
                                                if (reccnt && dircnt && !hidcnt && !longcnt) {
                                                    strcpy(commands[1], "-rec");
                                                    list(commands, 2, aux_directory2, 1);
                                                } else if (reccnt && dircnt && !hidcnt && longcnt) {
                                                    strcpy(commands[1], "-rec");
                                                    strcpy(commands[2], "-long");
                                                    list(commands, 3, aux_directory2, 1);
                                                } else if (reccnt && dircnt && hidcnt && !longcnt) {
                                                    strcpy(commands[1], "-rec");
                                                    strcpy(commands[2], "-hid");
                                                    list(commands, 3, aux_directory2, 1);
                                                } else if (reccnt && dircnt && hidcnt && longcnt) {
                                                    strcpy(commands[1], "-rec");
                                                    strcpy(commands[2], "-hid");
                                                    strcpy(commands[3], "-long");
                                                    list(commands, 4, aux_directory2, 1);
                                                }
                                            }
                                        }
                                        closedir(dr2);
                                    }
                                }
                            }
                        }
                    }
                    closedir(dr);
                    dr = opendir(current_directory);
                    not_found = 0;
                } //for
            } else {
                if (longcnt && reccnt && hidcnt) i = 4;
                else if (longcnt && reccnt && !hidcnt) i = 3;
                else if (longcnt && !reccnt && hidcnt) i = 3;
                else if (!longcnt && reccnt && hidcnt) i = 3;
                else if (longcnt && !reccnt && !hidcnt) i = 2;
                else if (!longcnt && !reccnt && hidcnt) i = 2;
                else if (!longcnt && reccnt && !hidcnt) i = 2;
                else if (!longcnt && !reccnt && !hidcnt) i = 1;

                for (; i < ntrozos; i++) {
                    if (!longcnt) {
                        while ((de = readdir(dr)) != NULL) {
                            if (!hidcnt && strncmp(de->d_name, ".", 1) ==
                                           0);                                                      //no se muestran los que empiezan por '.'
                            else {
                                if (strcmp(de->d_name, commands[i]) == 0) {
                                    stat(de->d_name, &fileStat);
                                    not_found++;

                                    printf("%10jd    %s\n", (intmax_t) fileStat.st_size, de->d_name);
                                }
                            }
                        }
                        if (not_found == 0)
                            printf("****error al acceder a %s: No such file or directory\n", commands[i]);
                        not_found = 0;
                    } else {
                        while ((de = readdir(dr)) != NULL) {
                            if (!hidcnt &&
                                strncmp(de->d_name, ".", 1) == 0); //no se muestran los que empiezan por '.'
                            else {
                                if (strcmp(de->d_name, commands[i]) == 0) {
                                    stat(de->d_name, &fileStat);
                                    not_found++;

                                    strcpy((char *) aux, ctime((const time_t *) &fileStat.st_mtim));
                                    TrocearCadena((char *) aux, fecha);

                                    struct passwd *nameid;
                                    nameid = getpwuid(fileStat.st_uid);
                                    struct group *grp;
                                    grp = getgrgid(fileStat.st_gid);

                                    if (de->d_type == DT_LNK) {
                                        char *buf;
                                        ssize_t bufsize, nbytes;
                                        bufsize = fileStat.st_size + 1;
                                        if (fileStat.st_size == 0)
                                            bufsize = MAXDIR;
                                        buf = malloc(bufsize);
                                        if (buf == NULL) {
                                            perror("malloc");
                                            exit(EXIT_FAILURE);
                                        }
                                        nbytes = readlink(de->d_name, buf, bufsize);


                                        printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s -> %.*s\n",
                                               fecha[0], fecha[1],
                                               fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name,
                                               grp->gr_name,
                                               ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
                                               (uintmax_t) fileStat.st_nlink, de->d_name, (int) nbytes, buf);
                                        free(buf);
                                    } else
                                        printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s\n", fecha[0],
                                               fecha[1],
                                               fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name,
                                               grp->gr_name,
                                               ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
                                               (uintmax_t) fileStat.st_nlink, de->d_name);

                                }
                            }
                        }
                        if (not_found == 0)
                            printf("****error al acceder a %s: No such file or directory\n", commands[i]);
                        not_found = 0;
                    }
                    closedir(dr);
                    dr = opendir(".");
                } //for
            }
        }
    }
    closedir(dr);
}

void memory(char* commands[MAXCOM], int ntrozos, Lista memL){

    if(ntrozos == 1 || (ntrozos == 2 && strcmp(commands[1], "-dealloc") == 0)) PrintList(memL);

    if(ntrozos == 2){
        if (strcmp(commands[1], "-allocate") == 0) PrintList(memL);
        else if (strcmp(commands[1], "-show-vars") == 0) showVars();
        else if (strcmp(commands[1], "-dopmap") == 0) Cmd_dopmap();
        else if (strcmp(commands[1], "-show-funcs") == 0){
            showProgFuncs();
            showCFuncs();
        }
        else if (strcmp(commands[1], "-show") == 0) {
            showProgFuncs();
            showVars();
        }
    }

    if(ntrozos == 3){
        if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-malloc") == 0) PrintMalloc(memL);
        else if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-mmap") == 0) PrintMmap(memL);
        else if (strcmp(commands[1], "-allocate") == 0 && (strcmp(commands[2], "-createshared") == 0 || strcmp(commands[2], "-shared") == 0)) PrintShared(memL);

        if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-malloc") == 0 ) PrintMalloc(memL);
        else if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-mmap") == 0 ) PrintMmap(memL);
        else if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-shared") == 0 ) PrintShared(memL);
        else if (strcmp(commands[1], "-dealloc") == 0) memoryDeallocAddr(commands[2], memL);

        if (strcmp(commands[1], "-deletekey") == 0){
            char *args[1];
            args[0]=commands[2];
            Cmd_deletekey(args);
        }

        if (strcmp(commands[1], "-show") == 0) show(commands[2], memL);

    }

    if(ntrozos == 4){
        if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-malloc") == 0 ) memoryAllocateMalloc(atoi(commands[3]), memL);
        else if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-shared") == 0 ) memoryAllocateShared(atoi(commands[3]), memL);
        if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-malloc") == 0 ) memoryDeallocMalloc(atoi(commands[3]), memL);
        if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-mmap") == 0 ) memoryDeallocMmap(commands[3], memL);
        if (strcmp(commands[1], "-dealloc") == 0 && strcmp(commands[2], "-shared") == 0 ) memoryDeallocShared(atoi(commands[3]), memL);
    }

    if(ntrozos == 5){
        if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-mmap") == 0) {
            char *args[2];
            args[0]=commands[3];
            args[1]=commands[4];
            Cmd_AllocateMmap (args, memL);
        }
        else if (strcmp(commands[1], "-allocate") == 0 && strcmp(commands[2], "-createshared") == 0) {
            char *args[2];
            args[0]=commands[3];
            args[1]=commands[4];
            Cmd_AllocateCreateShared(args, memL);
        }

    }
}

void procesarEntrada(char* entry, List L, Lista memL, int I) {

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
            historic("", L, memL, I);
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
            historic(commands[1], L, memL, I);
            validCommand = true;
        }
        else if(strcmp(commands[0], "create") == 0) {
            create(commands[1],"", ntrozos);
            validCommand = true;
        }
        else if(strcmp(commands[0], "memdump") == 0) {
            memdump(25, commands[1]);
            validCommand = true;
        }
        else if (strcmp(commands[0], "memfill") == 0){
            memfill(128, "0x42", commands[1]);
            validCommand = true;
        }
        else if (strcmp(commands[0], "recurse") == 0){
            doRecursiva(atoi(commands[1]));
            validCommand = true;
        }
        else validCommand = false;
    }

    else if(ntrozos == 3){
        if(strcmp(commands[0], "create") == 0) {
            create(commands[1],commands[2], ntrozos);
            validCommand = true;
        }
        else if(strcmp(commands[0], "memdump") == 0) {
            memdump(atoi(commands[2]), commands[1]);
            validCommand = true;
        }
        else if (strcmp(commands[0], "memfill") == 0) {
            if (strchr(commands[2], 'x') == NULL)
                memfill(atoi(commands[2]), "0x42", commands[1]);
            else memfill(128, commands[2], commands[1]);
            validCommand = true;
        }
    }

    else if(ntrozos == 4){
        if (strcmp(commands[0], "memfill") == 0){
            memfill(atoi(commands[2]), commands[3], commands[1]);
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

    if(strcmp(commands[0], "memory")==0){
        memory((char **)commands, ntrozos, memL);
        validCommand = true;
    }

    if(strcmp(commands[0], "readfile")==0){
        if (ntrozos==1 || ntrozos==2) printf("faltan parametros\n");
        else if (ntrozos==3 || ntrozos ==4) {
            char *args[3];
            args[0]=commands[1];    //file
            args[1]=commands[2];    //addr
            args[2]=commands[3];    //cont

            readFile(args);
        }
        validCommand = true;
    }

    if(strcmp(commands[0], "writefile")==0){
        if(ntrozos == 4){                                               //Opción SIN -o
            char *args[4];
            args[0]=commands[1];    //fich
            args[1]=commands[2];    //addr
            args[2]=commands[3];    //cont
            args[3]=NULL;           //-o

            writeFile(args);

        } else if((ntrozos == 5) && strcmp("-o", commands[1]) == 0){    //Opción CON -o
            char *args[4];
            args[0]=commands[2];    //fich
            args[1]=commands[3];    //addr
            args[2]=commands[4];    //cont
            args[3]=commands[1];    //-o

            writeFile(args);
        } else printf("faltan parametros\n");
        validCommand = true;
    }

    if (validCommand == false || (ntrozos == 0)) {
        printf("%s no encontrado\n", commands[0]);
    }

}

int main() {

    //creamos la lista
    List L = malloc(sizeof(Node));
    CreateListHistoric(L);
    Lista memL = malloc(sizeof(Node));
    CreateList(memL);
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
        InsertElementHistoric(L, &E[I]);

        if ((strcmp(entry, "quit\n") == 0) || (strcmp(entry, "end\n") == 0) || (strcmp(entry, "exit\n") == 0)) {
            terminado = true;
        } else {
            procesarEntrada(entry, L, memL, I);
        }
        I++;
    }
    RemoveHistoric(L);
    free(L);
    RemoveList(memL);
    free(memL);

    return 0;
}
