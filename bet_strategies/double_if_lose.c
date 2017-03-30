#include "../bet_strategies.h"

#define AMOUNT 0

Player *double_if_lose(Player *p, Table *t){

  if(!p || !t){
    fprintf(stderr, "double_if_lose: error arguments pointin NULL");
    return NULL;
  }
  /*Funciones a utilizar
   *player_getLastBet
   *player_removeCash
   *funcion que diga si se ha perdido, empatado o perdido
   */

  //Ver si en la ultima partida se ha perdido, ganado o empatado

  //Si se ha ganado, se apuesta la cantidad inicial: (amount)

  //Si se ha empatado, se apuesta lo mismo que en la apuesta anterior

  //Si se ha perdido, se dobla la apuesta anterior

  return p;
}
