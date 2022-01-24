shell: shell.c list.h lista_historic.h p2functions.h list.o lista_historic.o p2functions.o
	gcc -o shell shell.c list.o lista_historic.o p2functions.o

list.o: list.h list.c
	gcc -c list.c

lista_historic.o: lista_historic.h lista_historic.c
	gcc -c lista_historic.c
	
p2functions.o: p2functions.h p2functions.c
	gcc -c p2functions.c	
