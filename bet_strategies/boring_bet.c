#include "../bet_strategies.h"

Player *boring_bet(Player* p, Table* t){

  if(!p || !t){
    fprintf(stderr, "boring_bet: error arguments pointing NULL");
    return NULL;
  }
    p=player_removeCash(p, INITIAL_AMOUNT);
    p=player_setLastBet(p, INITIAL_AMOUNT);
    hand_setCurrentBet(player_getHand(p, 0), INITIAL_AMOUNT);
    if(!p){
        fprintf(stderr,"boring_bet: Error removing cash from player.\n");
        return NULL;
    }
    return p;
}
