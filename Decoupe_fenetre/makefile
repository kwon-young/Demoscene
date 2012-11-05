CC = gcc -std=c99
CFLAGS = -pedantic -Wall -Wextra -Wc++-compat
LDFLAGS = -lm

test : pi.o 
	$(CC) pi.o  -o test $(LDFLAGS)

pi.o : pi.c pi.h
	$(CC) $(CFLAGS) -c pi.c -o pi.o

clear :
	rm -f *.o *~ *.log *.aux test
