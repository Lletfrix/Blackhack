CC=gcc
CFLAGS=-g -Wall

OBJECTS=deck.o hand.o crupier.o player.o

.PHONY: all
all: $(OBJECTS) clean

deck.o: deck.h deck.c
	$(CC) $(CFALGS) -c deck.c

hand.o: hand.h hand.c
	$(CC) $(CFLAGS) -c hand.c

crupier.o: crupier.h crupier.c
	$(CC) $(CFLAGS) -c crupier.c

player.o: hand.h hand.c
	$(CC) $(CFLAGS) -c player.c


.PHONY: clean
clean:
	rm -rf $(OBJECTS) *.dSYM
