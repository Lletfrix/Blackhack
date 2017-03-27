/* main.c*/
#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

// load strategies
#include "bet_strategies.h"
#include "play_strategies.h"

#define NUMPLAYERS 4

int main(){
  Player *players[NUMPLAYERS];
  for (int i = 0; i < NUMPLAYERS; i++){
    // pasar funciones vacías para que de momento compile
    players[i] = player_ini(never_bets, play_do_nothing);
    if(!players[i]){
      fprintf(stderr, "main: player_ini: error allocating memory of player %d\n", i);
      for(int j=0;j<i;j++){
        player_destroy(players[j]);
      }
    }
  }
  Crupier *crupier = crupier_ini();
  if(!crupier){
    fprintf(stderr, "main: crupier_ini: error allocating memory of crupier\n");
    for(int i=0;i<NUMPLAYERS;i++){
      player_destroy(players[i]);
    }
  }
  Deck *deck = deck_ini();
  if(!deck){
    fprintf(stderr, "main: deck_ini: error allocating memory of deck\n");
    crupier_destroy(crupier);
    for(int i=0;i<NUMPLAYERS;i++){
      player_destroy(players[i]);
    }
  }

  for (int k=0;k<1;k++) {

    /* apostar*/
    for (int i = 0; i < NUMPLAYERS; i++) {
      players[i] = player_bet(players[i]);
    }

    /* repartir primera carta */
  for (int i = 0; i < NUMPLAYERS; i++)
  	players[i]=player_addCardToHand(players[i], 0, deck_draw(deck));
  }
    /* repartir 1 carta al crupier */
  crupier=crupier_addCard(crupier, deck_draw(deck));

    /*repartir segunda carta*/

  for (int i = 0; i < NUMPLAYERS; i++){
  	players[i]=player_addCardToHand(players[i], 0, deck_draw(deck));
  }

  /* juegan los jugadores */
  for (int i = 0; i < NUMPLAYERS; i++) {
  	players[i] = player_play(players[i]);
  }

  /*juega el crupier*/
  crupier=crupier_play(crupier, deck);

  /*comparar manos y repartir premios*/
  Hand *crupier_hand = crupier_getHand(crupier);

  // de momento asumimos que cada jugador solo tiene una mano
  for (int i = 0; i < NUMPLAYERS; i++) {
      Hand *player_hand = player_getHand(players[i], 0);
      printf("player %d, %d", i, (int)hand_compare(crupier_hand, player_hand));
  }
}
