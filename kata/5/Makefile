CC=gcc
CFLAGS=-g -Wall -Wextra -lssl -lcrypto
TARGET=main

.PHONY: install clean

install:
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).c

clean:
	rm -f $(TARGET) bloom
