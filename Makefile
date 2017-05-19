CC=gcc
CFLAGS=-Wall -std=c99 -g
GAMES= simulation real_player_game godmode_play

SIM_OBJECTS=deck.o hand.o crupier.o player.o table.o probability.o simulation.o
RPG_OBJECTS=deck.o hand.o crupier.o player.o table.o probability.o real_player_game.o
GMP_OBJECTS=deck.o hand.o crupier.o player.o table.o probability.o godmode_play.o
BET_STRATEGIES=never_bets.o boring_bet.o double_if_win.o double_if_lose.o
PLAY_STRATEGIES=play_do_nothing.o play_like_crupier.o play_random.o play_standard_wo_sd.o play_basic.o play_basic_17S_DAS.o

.PHONY: all
all: $(GAMES)

#main: main.c $(OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	#$(CC) $(CFLAGS) -o main main.c $(OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

deck.o: deck.h deck.c
	$(CC) $(CFLAGS) -c deck.c

hand.o: hand.h hand.c
	$(CC) $(CFLAGS) -c hand.c

crupier.o: crupier.h crupier.c
	$(CC) $(CFLAGS) -c crupier.c

player.o: hand.h hand.c
	$(CC) $(CFLAGS) -c player.c

table.o: table.h table.c
	$(CC) $(CFLAGS) -c table.c

probability.o: probability.h probability.c
	$(CC) $(CFLAGS) -c probability.c

real_player_game.o: table.h player.h crupier.h hand.h deck.h probability.h bet_strategies.h play_strategies.h
	$(CC) $(CFLAGS) -c real_player_game.c

godmode_play.o: table.h player.h crupier.h hand.h deck.h probability.h bet_strategies.h play_strategies.h
	$(CC) $(CFLAGS) -c godmode_play.c

simulation.o: table.h player.h crupier.h hand.h deck.h probability.h bet_strategies.h play_strategies.h
	$(CC) $(CFLAGS) -c simulation.c

real_player_game: $(RPG_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o real_player_game $(RPG_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

godmode_play: $(GMP_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o godmode_play $(GMP_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

simulation: $(SIM_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o simulation $(SIM_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

# bet strategies
never_bets.o: bet_strategies/never_bets.c
	$(CC) $(CFLAGS) -c bet_strategies/never_bets.c

boring_bet.o: bet_strategies/boring_bet.c
	$(CC) $(CFLAGS) -c bet_strategies/boring_bet.c

double_if_win.o: bet_strategies/double_if_win.c
	$(CC) $(CFLAGS) -c bet_strategies/double_if_win.c

double_if_lose.o: bet_strategies/double_if_lose.c
	$(CC) $(CFLAGS) -c bet_strategies/double_if_lose.c

# play strategies
do_nothing.o: bet_strategies/never_bets.c
	$(CC) $(CFLAGS) -c bet_strategies/never_bets.c

play_do_nothing.o: play_strategies/play_do_nothing.c
	$(CC) $(CFLAGS) -c play_strategies/play_do_nothing.c

play_like_crupier.o: play_strategies/play_like_crupier.c
	$(CC) $(CFLAGS) -c play_strategies/play_like_crupier.c

play_random.o: play_strategies/play_random.c
	$(CC) $(CFLAGS) -c play_strategies/play_random.c

play_standard_wo_sd.o: play_strategies/play_standard_wo_sd.c
	$(CC) $(CFLAGS) -c play_strategies/play_standard_wo_sd.c

play_basic.o: play_strategies/play_basic.c
	$(CC) $(CFLAGS) -c play_strategies/play_basic.c

play_basic_17S_DAS.o: play_strategies/play_basic_17S_DAS.c
	$(CC) $(CFLAGS) -c play_strategies/play_basic_17S_DAS.c

.PHONY: clean
clean:
	rm -rf $(SIM_OBJECTS) $(RPG_OBJECTS) *.dSYM *.o real_player_game simulation godmode_play

.PHONY: write
write:
	for i in `seq 1 10`; do\
		./simulation 1000 $$i.txt;\
		sleep 1;\
	done
