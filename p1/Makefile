shell: shell.c delete.h list.h historicList.h delete.o list.o historicList.o
	gcc -o shell shell.c list.o delete.o historicList.o

delete.o: delete.h delete.c
	gcc -c delete.c

list.o: list.h list.c
	gcc -c list.c

historicList.o: historicList.h historicList.c
	gcc -c historicList.c
