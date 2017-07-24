#include "../bet_strategies.h"
#include "../macros.h"

Player* manual_bet(Player *p, Table *t){
  int cash;

  if(!p || !t){
    fprintf(stderr, "manual_bet: error arguments pointing NULL");
    return NULL;
  }
  fprintf(stdout, "Tienes %d rublos\n", player_getCash(p));
  fprintf(stdout, "¿Cuantos rublos quieres apostar?\n");
  fscanf(stdout, "%d", &cash);

  p=player_removeCash(p, cash);
  p=player_setLastBet(p, cash);
  hand_setCurrentBet(player_getHand(p, 0), cash);
  if(!p){
    fprintf(stderr,"manual_bet: Error removing cash from player.\n");
    return NULL;
  }
  return p;
}
