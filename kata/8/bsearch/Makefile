CC = gcc
# We use C99 just to have easy access to %zu print modifier.
FLAGS = -g -Wall -Wextra -pedantic -std=c99

OBJS = load.o

ifdef ComSpec
   # Windows.
   RM = del
else
   # Everything else.
   RM = rm -f
endif

.PHONY: install

install: all
	$(CC) -o main $(FLAGS) main.c $(OBJS)
	$(RM) *.o

all: $(OBJS)

%.o : %.c
	$(CC) -c $(FLAGS) $< -o $@

clean:
	$(RM) *.o main
