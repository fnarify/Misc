CC=gcc
CFLAGS=-g -Wall -Wextra -lssl -lcrypto
TARGET=main

.PHONY: install clean

install:
	$(CC) $(CFLAGS) $(TARGET).c

clean:
	rm -f bloom a.out
