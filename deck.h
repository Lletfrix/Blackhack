#ifndef DECK_H
#define DECK_H

#include "macros.h"

typedef struct _Deck Deck;

/* initialize deck, NULL if error */
Deck* deck_ini();

/* destroy deck */
void deck_destroy(Deck*);

/* draw a card, return rank, -1 if error */
int deck_draw(Deck*);

/* check if deck is empty, return false if error */
bool deck_isEmpty (Deck* );

/* gets current number of cards in a deck, returns -1 if error */
int deck_getNCurrentCards(Deck* );

/*restart all the cards and info of a deck*/
Deck* deck_restartDeck(Deck *);


#endif
