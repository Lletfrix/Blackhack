#include "../play_strategies.h"

Player play_like_crupier(Player *p, Deck *d){
  int *handValues;
  if (!p || !d) {
      fprintf(stderr, "play_like_crupier: player or deck pointing NULL\n");
      return NULL;
  }
  /*Suponiendo que el jugador ya tiene las dos primeras cartas*/

  /*Suponiendo que el jugador solo tiene una mano*/

  handValues = hand_getValues(player_getHand(p, 0));
  if (!handValues) {
      fprintf(stderr, "play_like_crupier: handValues pointing NULL\n");
      return NULL;
  }
  /*Añade cartas a la mano mientras el primer valor(As=11) sea menor que 17*/
  while (handValues[0] < 17) {
      p = player_addCardToHand(p,0, deck_draw(d));
      free(handValues);
      handValues = hand_getValues(player_getHand(p, 0));
      if (!handValues) {
          fprintf(stderr, "play_like_crupier: handValues pointing NULL\n");
          return NULL;
      }
  }
  /*Si no sobrepasa 21 devuelve c, si se pasa, comprueba que existe el segundo
   valor, (As=1) y pide cartas igual que antes*/
  if (handValues[0] > 21) {
      if (handValues[1] > 0) {
          while (handValues[1] < 17) {
              p = player_addCardToHand(p,0, deck_draw(d));
              free(handValues);
              handValues = hand_getValues(player_getHand(p, 0));
              if (!handValues) {
                  fprintf(stderr, "play_like_crupier: handValues pointing NULL\n");
                  return NULL;
              }
          }
          return c;
      }
}
