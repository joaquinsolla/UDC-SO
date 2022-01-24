shell: shell.c list.h lista_historic.h p2functions.h p3functions.h lista_procesos.h list.o lista_historic.o p2functions.o p3functions.o lista_procesos.o
	gcc -o shell shell.c list.o lista_historic.o p2functions.o p3functions.o lista_procesos.o

list.o: list.h list.c
	gcc -c list.c

lista_historic.o: lista_historic.h lista_historic.c
	gcc -c lista_historic.c
	
p2functions.o: p2functions.h p2functions.c
	gcc -c p2functions.c	
	
p3functions.o: p3functions.h p3functions.c
	gcc -c p3functions.c		
	
lista_procesos.o: lista_procesos.h lista_procesos.c
	gcc -c lista_procesos.c	
