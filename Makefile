CC=gcc
CFLAGS=-g -Wall

OBJECTS=deck.o hand.o crupier.o player.o
BET_STRATEGIES=never_bets.o
PLAY_STRATEGIES=play_do_nothing.o

.PHONY: all
all: main clean

main: main.c $(OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o main main.c $(OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

deck.o: deck.h deck.c
	$(CC) $(CFLAGS) -c deck.c

hand.o: hand.h hand.c
	$(CC) $(CFLAGS) -c hand.c

crupier.o: crupier.h crupier.c
	$(CC) $(CFLAGS) -c crupier.c

player.o: hand.h hand.c
	$(CC) $(CFLAGS) -c player.c

# bet strategies
never_bets.o: bet_strategies/never_bets.c
	$(CC) $(CFLAGS) -c bet_strategies/never_bets.c

# play strategies
do_nothing.o: bet_strategies/never_bets.c
	$(CC) $(CFLAGS) -c bet_strategies/never_bets.c

play_do_nothing.o: play_strategies/play_do_nothing.c
	$(CC) $(CFLAGS) -c play_strategies/play_do_nothing.c


.PHONY: clean
clean:
	rm -rf $(OBJECTS) *.dSYM *.o main
