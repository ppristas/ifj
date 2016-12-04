CFLAGS=-std=c99 -Wall -Wextra -Werror -pedantic -DNDEBUG -g
LDFLAGS=-lm
interpret: scaner.o main.o error.o stack.o parser.o cleaner.o buildin.o debug.o preced.o ial.o temp_tab.o stack_frame.o ilist.o interpret.o
	gcc $(CFLAGS) scaner.o main.o error.o stack.o parser.o cleaner.o buildin.o ial.o debug.o preced.o temp_tab.o stack_frame.o ilist.o  interpret.o -o interpret $(LDFLAGS)
clean:
	-rm *.o test	
tests:
	./Testy/test.sh
