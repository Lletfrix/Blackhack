/* main.c*/
#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

#define NUMPLAYERS 4

int main(){
  int i,j;
  Player *players[NUMPLAYERS];
  for (int i = 0; i < NUMPLAYERS; i++){
    players[i] = player_ini(NULL); // TODO: pasar estrategia
    if(!players[i]){
      fprintf(stderr, "main: player_ini: error allocating memory of player %d\n", i);
      for(j=0;j<i;j++){
        player_destroy(players[j]);
      }
    }
  }
  Crupier *crupier = crupier_ini();
  if(!crupier){
    fprintf(stderr, "main: crupier_ini: error allocating memory of crupier\n");
    for(i=0;i<NUMPLAYERS;i++){
      player_destroy(players[i]);
    }
  }
  Deck *deck = deck_ini();
  if(!deck){
    fprintf(stderr, "main: deck_ini: error allocating memory of deck\n");
    crupier_destroy(crupier);
    for(i=0;i<NUMPLAYERS;i++){
      player_destroy(players[i]);
    }
  }

  while (true) {

    /* apostar*/
    for (int i = 0; i < NUMPLAYERS; i++) {
      players[i] = player_bet(players[i]);
    }

    /* repartir primera carta */
  for (int i = 0; i < NUMPLAYERS; i++)
  	players[i]=player_addCardToHand(players[i], 0, deck_draw(deck));
  }
    /* repartir 1 carta al crupier */
  crupier=crupier_addCard(c, deck_draw(deck));

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
  int crupier_hand_values = hand_getValues(crupier_getHand(crupier));

  for (int i = 0; i < NUMPLAYERS; i++) {
    for (int j = 0; j < player_getNhands(players[i]); j++) {
      int *hand_values = hand_getValues(player_getHand(players[i], j));
      if (hand_values[0] <= 21) {
        // empate
        // gana jugador
        // gana casa




        }
    }
  }
}
