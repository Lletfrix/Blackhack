#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "macros.h"

typedef struct _Player Player;

/* function pointer type */
typedef Player *(*bet_function_type)(Player *);
typedef Player *(*play_function_type)(Player *);


/* initialize a player with only one hand */
Player* player_ini(bet_function_type, play_function_type);

/* destroy a player */
void player_destroy(Player*);

/* make a play */
Player* player_play(Player*);

/* adds a card to the hand */
Player* player_addCardToHand(Player*,int numHand, int rank);

/* adds cash to the player */
Player* player_addCash(Player*, int);

/* removes cash from the player */
Player* player_removeCash(Player*, int);

/* next functions allow access to different fields of Player */
int player_getTotalCards(Player*);

int player_getCash(Player*);

int player_getLastBet(Player*);

bool player_lastPlayWin(Player*);

int player_getNWin(Player*);

int player_getNPlayed(Player*);

Hand* player_getHand(Player*, int numHand);

int player_getNHands(Player*);
#endif
