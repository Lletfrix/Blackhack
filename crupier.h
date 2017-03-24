#ifndef CRUPIER_H
#define CRUPIER_H

#include "hand.h"
#include "macros.h"

typedef struct _Crupier Crupier;


/* initialize a crupier */
Crupier* crupier_ini();

/* destroy a crupier */
void crupier_destroy(Crupier*);

/* make a play */
Crupier* crupier_play (Crupier*);

/* adds a card to the hand */
Crupier* crupier_addCard(Crupier*, int rank);

/* gets crupier hand */
Hand* crupier_getHand(Crupier*);

#endif
