#ifndef PLAYER_H
#define PLAYER_H

#include "hand.h"
#include "macros.h"

typedef struct _Player Player;

#include "table.h"

/* function pointer type */
typedef Player *(*bet_function_type)(Player *, Table *);
typedef Player *(*play_function_type)(Player *, Table *);


/* initialize a player with only one hand */
Player* player_ini(bet_function_type, play_function_type);

/* destroy a player */
void player_destroy(Player*);

/*
 * Make a bet.
 */
Player* player_bet(Player*, Table*);

/* make a play */
Player* player_play(Player*, Table*);

/* adds a card to the hand */
Player* player_addCardToHand(Player*,int numHand, int rank);

/* adds cash to the player */
Player* player_addCash(Player*, int);

/* removes cash from the player */
Player* player_removeCash(Player*, int);

/* increase win, tie or lose and num played, cmpValue=0 (crupier wins), cmpValue=1(tie) cmpValue=2(player wins) */
Player* player_addGame (Player* , int cmpValue);

/* restart player hands */
Player* player_restartHands (Player*);

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
