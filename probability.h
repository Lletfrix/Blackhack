#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "hand.h"
#include "deck.h"
#include "player.h"
#include "crupier.h"
#include "table.h"

/*
 ********************************
 Posibles funciones a implementar
 ********************************
*/

//returns the probability of card
double probability_card(Deck *, int rank);

//returns the probability of all the cards
double * probability_listofcards(Deck *);

//return -1, 0, +1 used to count cards
int probability_countingHiLo(int rank);

//return -2,-1,0,1,2,3 used to count cards
int probability_countingUstonSS(int rank);

//return the probability of go over of a hand, return 2 values;
double *probability_goOver(Deck *, Hand *);
