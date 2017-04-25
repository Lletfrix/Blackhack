#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define N_CARDS_DECK 52
#define N_DECKS 6
#define N_RANKS 10
#define MAX_HANDS 5
#define MIN_DECK_WEIGHT 2
#define INITIAL_CASH 100
#define INITIAL_AMOUNT 1

typedef enum{
  LOSE, TIE, WIN, ERROR
}Peg;

typedef enum{
    FAIL, OK
}Status;
/*
 * Si está definida, compilamos con control de errores.
 */
#define DEBUG

#endif
