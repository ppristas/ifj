CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -DNDEBUG -g
LDFLAGS=-lm
scaner: scaner.o main.o
	gcc $(CFLAGS) scaner.o main.o -o scaner $(LDFLAGS)
clean:
	-rm *.o


