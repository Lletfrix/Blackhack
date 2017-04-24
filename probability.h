#include hand.h
#include deck.h
#include player.h
#include crupier.h
#include table.h

/*
 ********************************
 Posibles funciones a implementar
 ********************************
*/

//returns the probability of card
double probability_card(Deck *, int rank);

//returns the probability of all the cards
double * probability_listofcards(Table *);

//return -1, 0, +1 used to count cards
int probability_countingHiLo(int rank);

//return remaining numbers of ases
int probability_remainingAses(Deck *);
