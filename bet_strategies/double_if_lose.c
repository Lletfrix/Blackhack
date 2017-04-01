#include "../bet_strategies.h"

#define AMOUNT 10

Player *double_if_lose(Player *p, Table *t){
  Peg condition;

  if(!p || !t){
    fprintf(stderr, "double_if_lose: error arguments pointin NULL");
    return NULL;
  }
  /*Funciones a utilizar
   *player_getLastBet
   *player_removeCash
   *player_getLastPlay
   */

  //Ver si en la ultima partida se ha perdido, ganado o empatado
  condition=player_getLastPlay(p);
  if(condition<0 || condition>2){
    fprintf(stderr, "double_if_lose: error play_getLastPlay return ERROR\n");
    return NULL;
  }

  //Si se ha ganado, se apuesta la cantidad inicial: (amount)
  if(condition==WIN){
    player_removeCash(p, AMOUNT);
    player_setLastBet(p, AMOUNT);
    return p;
  }

  //Si se ha empatado, se apuesta lo mismo que en la apuesta anterior
  if(condition==TIE){
    player_removeCash(p, player_getLastBet(p));
    return p;
  }
    //Si se ha perdido, se dobla la apuesta anterior
  if(condition==LOSE){
    player_removeCash(p, 2*player_getLastBet(p));
    player_setLastBet(p, 2*player_getLastBet(p));
    return p;
  }

  /*Si se ha llegado hasta aqui, es que ha habido error, devuelve NULL*/
  fprintf(stderr, "double_if_lose: return error\n");
  return NULL;
}
