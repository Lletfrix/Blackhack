#define MAX_HANDS 4
#include "player.h"

struct _Player{
Hand *hand[MAX_HANDS];
int nTotalCards;
int cash;
int lastBet;
bool lastPlayWon;
int nWin;
int nPlay;
void (*bet)(Player*); 
};
