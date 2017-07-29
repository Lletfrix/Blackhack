#ifndef DECK_H
#define DECK_H


/*  ERRORS */
#include <math.h>
#define E_INVALID_DECK NAN

#include "macros.h"

typedef struct _Deck Deck;

/* initialize deck, NULL if error */
Deck* deck_ini(unsigned seed);

/* destroy deck */
void deck_destroy(Deck*);

/* draw a card, return rank, -1 if error */
int deck_draw(Deck*);

/* check if deck is empty, return false if error */
bool deck_isEmpty (Deck* );

/*  Removes the selected card from the deck. Returns NULL if impossible. */
Deck *deck_removeCard(Deck *d, int rank);

/* gets current number of cards in a deck, returns -1 if error */
int deck_getNCurrentCards(Deck* );

/* get current number of cards from the same rank in a deck */
int deck_getNCardsofRank(Deck*,  int rank);

/*restart all the cards and info of a deck*/
Deck* deck_restartDeck(Deck *);

/*  Returns an array of the percentage of cards of each rank remaining in the deck. Array must be free'd by the user. */
double *deck_distribution(Deck *);

void deck_print(FILE*, Deck*);

float deck_runningCount(Deck *d);

float deck_realCount(Deck *d);

#endif
