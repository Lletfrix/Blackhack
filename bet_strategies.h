#include "player.h"
#include "table.h"

/**
 * Esta estrategia nunca apuesta.
 */
Player *never_bets(Player *, Table *);

Player *double_if_lose(Player *, Table *);

Player *double_if_win(Player *, Table *);

Player *boring_bet(Player*, Table*);

Player *manual_bet(Player *, Table *);

Player *counting_bet(Player *, Table*);
