/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 05/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "list.h"

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
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
        if (dr ==
            NULL)                                                                                                       //opendir returns NULL if couldn't open directory
        {
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


                while ((de = readdir(dr)) != NULL) {
                    if (!hidcnt && strncmp(de->d_name, ".", 1) == 0)
                        continue;                              //no se muestran los que empiezan por '.'

                    strcpy(aux_directory, current_directory);
                    strcat(aux_directory, "/");
                    strcat(aux_directory, de->d_name);
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
                                   fecha[1],
                                   fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name, grp->gr_name,
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
            } else {                                                                                                    //no tiene -rec

                printf("********* %s\n", current_directory);
                while ((de = readdir(dr)) != NULL) {
                    if (!hidcnt && strncmp(de->d_name, ".", 1) ==
                                   0);
                    else {
                        stat(de->d_name, &fileStat);

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
                                       fecha[1],
                                       fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name, grp->gr_name,
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
                }
            }
        }
    }

    else {                                                                                                              ////HAY NOMBRES////

        if (dircnt) {
            if (dr ==NULL){                                                                                         // opendir returns NULL if couldn't open directory
                printf("Could not open current directory\n");
            }
            else {
                int i;
                if(reccnt && dircnt && !hidcnt && !longcnt) i=3;
                else if(reccnt && dircnt && !hidcnt &&longcnt) i=4;
                else if(reccnt && dircnt && hidcnt && !longcnt) i=4;
                else if(reccnt && dircnt && hidcnt && longcnt) i=5;
                else if(!reccnt && dircnt && !hidcnt &&longcnt) i=3;
                else if(!reccnt && dircnt && hidcnt && !longcnt) i=3;
                else if(!reccnt && dircnt && hidcnt && longcnt) i=4;
                else if(!reccnt && dircnt && !hidcnt && !longcnt) i=2;

                for (i; i < ntrozos; i++) {
                    int y = i;
                    for(y; y < ntrozos; y++){
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
                    if(not_found == 0)
                        printf("****error al acceder a %s: No such file or directory\n", commands[i]);
                    closedir(dr);
                    dr = opendir(current_directory);

                    if(not_found != 0) {
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
                                                       fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name,
                                                       grp->gr_name,
                                                       ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
                                                       (uintmax_t) fileStat.st_nlink, de2->d_name, (int) nbytes, buf);
                                                free(buf);
                                            } else
                                                printf("%s %s %s %s %9ju    %s    %s   %s %10jd  (%jd)  %s\n", fecha[0],
                                                       fecha[1],
                                                       fecha[2], fecha[4], (uintmax_t) fileStat.st_ino, nameid->pw_name,
                                                       grp->gr_name,
                                                       ConvierteModo2(fileStat.st_mode), (intmax_t) fileStat.st_size,
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
            }
        }

            ////list -long -rec ... o list -rec ...
            ////list -long ... o list ...
        else {

            if (dr ==
                NULL)                                                                                         // opendir returns NULL if couldn't open directory
            {
                printf("Could not open current directory\n");
            } else {
                int i;
                if (longcnt && reccnt && hidcnt) i = 4;
                else if (longcnt && reccnt && !hidcnt) i = 3;
                else if (longcnt && !reccnt && hidcnt) i = 3;
                else if (!longcnt && reccnt && hidcnt) i = 3;
                else if (longcnt && !reccnt && !hidcnt) i = 2;
                else if (!longcnt && !reccnt && hidcnt) i = 2;
                else if (!longcnt && reccnt && !hidcnt) i = 2;
                else if (!longcnt && !reccnt && !hidcnt) i = 1;

                for (i ; i < ntrozos; i++) {
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
                        if(not_found == 0)
                            printf("****error al acceder a %s: No such file or directory\n", commands[i]);
                        not_found = 0;
                    } else {
                        while ((de = readdir(dr)) != NULL) {
                            if (!hidcnt && strncmp(de->d_name, ".", 1) == 0); //no se muestran los que empiezan por '.'
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
                        if(not_found == 0)
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
