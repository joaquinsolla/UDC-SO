/*
* AUTHOR 1: JOAQUÍN SOLLA VÁZQUEZ LOGIN 1: joaquin.solla
* AUTHOR 2: SAÚL LEYVA SANTARÉN: saul LOGIN 2: leyva.santaren
* GROUP: 2.5
* DATE: 05/11/2020
* IMPLEMENTATION: (0) LINKED LIST
*/

#include "delete.h"

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

void delete (char* subCommand[], int ntrozos) {

    if (ntrozos == 1 && strcmp(subCommand[0], "./delete")==0) {                                                           //SHOW FILES

        DIR *d;
        int sz;
        struct dirent *dir;
        d = opendir(".");
        printf("********* .\n");

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
                printf("********* .\n");

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