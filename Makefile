CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -DNDEBUG -g
LDFLAGS=-lm
interpret: scaner.o main.o error.o stack.o
	gcc $(CFLAGS) scaner.o main.o error.o stack.o -o interpret $(LDFLAGS)
clean:
	-rm *.o


