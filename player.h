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
Player* player_addCash(Player*, long);

/* removes cash from the player */
Player* player_removeCash(Player*, long);

/* return win condition of each hand, by order */
Peg* player_handsCondition(Crupier*, Player*);

/* increase win, tie or lose and num played, cmpValue=0 (crupier wins), cmpValue=1(tie) cmpValue=2(player wins) */
Player* player_addGame (Player* , Peg);

/* restart player hands */
Player* player_restartHands (Player*);

/* set the field of player LastBet*/
Player *player_setLastBet(Player *, long);

/* refresh Win, Tie or Lose streak of a player*/
Player *player_refreshStreak(Player *, Peg);

/* split player i-hand */
Player *player_splitHand(Player*, int numHand);

/* next functions allow access to different fields of Player */
int player_getTotalCards(Player*);

long player_getCash(Player*);

long player_getLastBet(Player*);

Peg player_getLastPlay(Player*);

int player_getNWin(Player*);

int player_getNTie(Player*);

int player_getNLost(Player*);

int player_getNPlayed(Player*);

double player_getWinRatio(Player *);

double player_getTieRatio(Player *);

double player_getLostRatio(Player *);

Hand* player_getHand(Player*, int numHand);

int player_getNHands(Player*);

int player_getCurrentWinStreak(Player *);

int player_getCurrentTieStreak(Player *);

int player_getCurrentLoseStreak(Player *);

int player_getMaxWinStreak(Player *);

int player_getMaxTieStreak(Player *);

int player_getMaxLoseStreak(Player *);

#endif
