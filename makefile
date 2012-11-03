# makefile du Labo2_Blocs

#definition du compilateur
CC=gcc -std=c99

#definition des options de compilation
CFLAGS=-pedantic -Wall -Wextra -Werror

#definition des options d'edition des liens
LDFLAGS=

#definition des cibles avec leurs dependances et les regles de production

all: main

# editions des liens
main: wave.o
	$(CC) $(LDFLAGS) wave.o -o wave

#compilations
wave.o: main.c
	$(CC) $(CFLAGS) -c main.c -o wave.o

#nettoyage
clean:
	erase /f *.o *~ main

0xAC1DE
