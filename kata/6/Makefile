CC = gcc
# We use C99 just to have easy access to %zu print modifier.
FLAGS=-g -Wall -Wextra -pedantic -std=c99

ifdef ComSpec
   # Windows.
   RM = del.bat
else
   # Everything else.
   RM = rm -f
endif

.PHONY: install

install: hashmap anagram
	$(CC) -o main $(FLAGS) main.c anagram.o hashmap.o
	$(RM) *.o

hashmap:
	$(CC) $(FLAGS) -c hashmap.c

anagram:
	$(CC) $(FLAGS) -c anagram.c

clean:
	 $(RM) *.o main
