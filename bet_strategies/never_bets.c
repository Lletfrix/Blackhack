#include "../bet_strategies.h"

Player *never_bets(Player *p, Table *t){
    player_setLastBet(p, 0);
    return p;
}
