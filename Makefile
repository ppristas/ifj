CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -DNDEBUG -g
LDFLAGS=-lm
scaner: scaner.o main.o error.o
	gcc $(CFLAGS) scaner.o main.o error.o -o scaner $(LDFLAGS)
clean:
	-rm *.o


