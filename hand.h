#ifndef HAND_H
#define HAND_H

#include "macros.h"

typedef struct _Hand Hand;

typedef enum {crupier_wins, tie, player_wins, compare_error} hand_compare_result;

/* initialize a hand, return NULL if error */
Hand* hand_ini();

/* destroy a hand */
void hand_destroy(Hand*);

/*  insert a card on the hand, return NULL if error */
Hand* hand_insertCard(Hand*, int rank);

/* get the value of the Hand, creating memory for the array, return NULL if error.
 * array is formed by two integers. array[0] is max possible value (AS=11) or the only
 * possible value if 0 or 12+ ases. array[1] is lowest possible value or -1 if there's only
 * one possible value.*/

int* hand_getValues(Hand*);

/* divide the hand, returning and allocating memory for the new hand and modifying the original one 
 * return NULL if error */
Hand* hand_divide(Hand* original);

/* gets the current number of cards, return -1 if error */
int hand_getNumCards(Hand*);

/* gets the array of cards,return NULL if error */
int* hand_getCards(Hand*);


/*
 * Compara las manos de un crupier y un jugador.
 */
hand_compare_result hand_compare(Hand *crupier_hand, Hand *player_hand);

#endif

