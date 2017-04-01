#include "../bet_strategies.h"

#define AMOUNT 1
#define MAX_ESTIMATED_WIN_STREAK 3

Player *double_if_win(Player *p, Table *t){
  Peg condition;

  if(!p || !t){
    fprintf(stderr, "double_if_win: error arguments pointin NULL");
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
    fprintf(stderr, "double_if_win: error play_getLastPlay return ERROR\n");
    return NULL;
  }

  //Si se ha ganado, se dobla la apuesta anterior, salvo que la racha de
  //victorias sea mayor que MAX_ESTIMATED_WIN_STREAK, que apuesta AMOUNT

  if(condition==WIN){
    if(player_getCurrentWinStreak(p)<MAX_ESTIMATED_WIN_STREAK){
      player_removeCash(p, 2*player_getLastBet(p));
      player_setLastBet(p, 2*player_getLastBet(p));
        return p;
    }
    player_removeCash(p, AMOUNT);
    player_setLastBet(p, AMOUNT);
    return p;
  }

  //Si se ha empatado, se apuesta lo mismo que en la apuesta anterior
  if(condition==TIE){
    player_removeCash(p, player_getLastBet(p));
    return p;
  }
    //Si se ha perdido, se apuesta la cantidad inicial: AMOUNT
  if(condition==LOSE){
    player_removeCash(p, AMOUNT);
    player_setLastBet(p, AMOUNT);
    return p;
  }

  /*Si se ha llegado hasta aqui, es que ha habido error, devuelve NULL*/
  fprintf(stderr, "double_if_win: return error\n");
  return NULL;
}
