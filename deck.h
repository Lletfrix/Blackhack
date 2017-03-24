#ifndef DECK_H
#define DECK_H

typedef struct _Deck Deck;
define N_DECKS 6
define N_RANKS 10

/* initialize deck */
Deck* deck_ini(unsigned int);

/* destroy deck */
void deck_destroy(Deck*);

/* draw a card, return rank */
int deck_draw(Deck*);

#endif
