CC=gcc
CFLAGS=-Wall -std=c99 -g

EXECS=simulation real_player_game godmode_play manual_mode

COMMON_OBJS=deck.o hand.o crupier.o player.o table.o probability.o
SIM_OBJECTS=$(COMMON_OBJS) simulation.o
RPG_OBJECTS=$(COMMON_OBJS) real_player_game.o
GMP_OBJECTS=$(COMMON_OBJS) godmode_play.o
MMO_OBJECTS=$(COMMON_OBJS) manual_mode.o
BET_STRATEGIES=never_bets.o boring_bet.o double_if_win.o double_if_lose.o manual_bet.o
PLAY_STRATEGIES=play_do_nothing.o play_like_crupier.o play_random.o play_standard_wo_sd.o play_basic.o play_basic_17S_DAS.o play_manual.o play_basic_17S_DAS_matrix.o

.PHONY: all
all: $(EXECS) clear

# BASE OBJECTS
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


# pre-executable libraries
real_player_game.o: real_player_game.c
	$(CC) $(CFLAGS) -c real_player_game.c

godmode_play.o: godmode_play.c
	$(CC) $(CFLAGS) -c godmode_play.c

simulation.o: simulation.c
	$(CC) $(CFLAGS) -c simulation.c

manual_mode.o: manual_mode.c
	$(CC) $(CFLAGS) -c manual_mode.c

# actual executables, everyone of these should be in $(EXECS)
real_player_game: $(RPG_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o real_player_game $(RPG_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

godmode_play: $(GMP_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o godmode_play $(GMP_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

simulation: $(SIM_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o simulation $(SIM_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

manual_mode: $(MMO_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)
	$(CC) $(CFLAGS) -o manual_mode $(MMO_OBJECTS) $(BET_STRATEGIES) $(PLAY_STRATEGIES)

# bet strategies
never_bets.o: bet_strategies/never_bets.c
	$(CC) $(CFLAGS) -c bet_strategies/never_bets.c

boring_bet.o: bet_strategies/boring_bet.c
	$(CC) $(CFLAGS) -c bet_strategies/boring_bet.c

double_if_win.o: bet_strategies/double_if_win.c
	$(CC) $(CFLAGS) -c bet_strategies/double_if_win.c

double_if_lose.o: bet_strategies/double_if_lose.c
	$(CC) $(CFLAGS) -c bet_strategies/double_if_lose.c

manual_bet.o: bet_strategies/manual_bet.c
	$(CC) $(CFLAGS) -c bet_strategies/manual_bet.c

# play strategies
do_nothing.o: play_strategies/play_do_nothing.c
	$(CC) $(CFLAGS) -c play_strategies/play_do_nothing.c

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

play_basic_17S_DAS_matrix.o: play_strategies/play_basic_17S_DAS_matrix.c
	$(CC) $(CFLAGS) -c play_strategies/play_basic_17S_DAS_matrix.c

play_manual.o: play_strategies/play_manual.c
	$(CC) $(CFLAGS) -c play_strategies/play_manual.c


.PHONY: clean
clean:
	rm -rf *.dSYM *.o $(EXECS)

.PHONY: clear
clear:
	rm  -rf *.dSYM *.o
