#ifndef HAND_H
#define HAND_H

typedef struct _Hand Hand;

/* initialize a hand */
Hand* hand_ini();

/* destroy a hand */
void hand_destroy (Hand* );

/*  insert a card on the hand */
Hand* hand_insertCards (int rank);

/* get the value of the Hand */
int* hand_getValues(Hand* );

/* divide the hand */
Hand* hand_divide(Hand* original);

/* gets the current number of cards */
int hand_getNumCards(Hand*);

/* gets the array of cards */
int* hand_getCards(Hand*);

#endif

