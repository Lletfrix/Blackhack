#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/*  Deck defaults */
#define N_CARDS_DECK 52
#define N_DECKS 6
#define N_RANKS 10

/*  Playing defaults */
#define MAX_HANDS 3
#define MIN_DECK_WEIGHT 2
#define INITIAL_CASH 1000
#define INITIAL_AMOUNT 10

/*  Type for "Pierde, empata o gana" */
typedef enum {
  LOSE, TIE, WIN, ERROR
} Peg;

typedef enum {
    FAIL, OK
} Status;

#endif
