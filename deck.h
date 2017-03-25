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

#endif
