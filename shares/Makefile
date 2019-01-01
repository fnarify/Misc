CC=gcc
CFLAGS=-Wall -Wextra -g
SHARE=share
EXCHANGE=exchange
STOCK=stock

.PHONY: share exchange stock

share:
	$(CC) -o $(SHARE) $(CFLAGS) $(SHARE).c

exchange:
	$(CC) -o $(EXCHANGE) $(CFLAGS) $(EXCHANGE).c

stock:
	$(CC) -o $(STOCK) $(CFLAGS) $(STOCK).c

clean:
	rm -f $(SHARE) $(EXCHANGE) $(STOCK)
