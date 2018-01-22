#include "../bet_strategies.h"

Player *counting_bet(Player* p, Table* t){

  if(!p || !t){
    fprintf(stderr, "counting_bet: error arguments pointing NULL");
    return NULL;
  }
  int bet = (int) INITIAL_CASH/UNITS;
  bet = (bet < MIN_TABLE_BET) ? MIN_TABLE_BET : bet;
  Deck *d;
  d=table_getDeck(t);

  int true_count = (int) deck_trueCount(d);

  if (true_count == 2){
    bet *= 2;
  }else if (true_count == 3){
    bet *= 4;
  }else if (true_count == 4){
    bet *= 8;
  }else{
    bet *=12;
  }

  p=player_removeCash(p, bet);
  p=player_setLastBet(p, bet);
  hand_setCurrentBet(player_getHand(p, 0), bet);
  if(!p){
      fprintf(stderr,"counting_bet: Error removing cash from player.\n");
      return NULL;
  }
  return p;
}
