#define MAX_HANDS 4
#include "player.h"

struct _Player {
    Hand *hand[MAX_HANDS];
    int nTotalCards;
    int cash;
    int lastBet;
    bool lastPlayWin;
    int nWin;
    int nPlay;
    bet_function_type f1;
};
