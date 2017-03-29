#include "../play_strategies.h"

Player *play_random(Player p, Table *t){
  int *handValues, yOnCard=1;
  if (!p || !t) {
      fprintf(stderr, "play_random: player or table pointing NULL\n");
      return NULL;
  }
  // get Deck from table
  Deck *d = table_getDeck(t);

  /*Suponiendo que el jugador ya tiene las dos primeras cartas*/

  /*Suponiendo que el jugador solo tiene una mano*/

  handValues = hand_getValues(player_getHand(p, 0));
  if (!handValues) {
      fprintf(stderr, "play_like_crupier: handValues pointing NULL\n");
      return NULL;
  }
  while(handValues[0]<21 && yOnCard==1){
    p = player_addCardToHand(p,0, deck_draw(d));
    free(handValues);
    handValues = hand_getValues(player_getHand(p, 0));
    if (!handValues) {
        fprintf(stderr, "play_randomr: handValues pointing NULL\n");
        return NULL;
    }
    yOnCard=rand()%2;
  }
  yOnCard=1;

  if (handValues[0] > 21) {
      if (handValues[1] > 0) {
          while (handValues[1] < 21 && yOnCard==1) {
              p = player_addCardToHand(p,0, deck_draw(d));
              free(handValues);
              handValues = hand_getValues(player_getHand(p, 0));
              if (!handValues) {
                  fprintf(stderr, "play_random: handValues pointing NULL\n");
                  return NULL;
              }
              yOnCard=rand()%2;
          }
          return p;
      }
    }

  return p;
}
