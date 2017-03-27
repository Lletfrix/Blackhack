
#include "player.h"
#include "hand.h"


struct _Player {
    Hand *hand[MAX_HANDS];
    int nTotalHands;
    int nTotalCards;
    int cash;
    int lastBet;
    bool lastPlayWin;
    int nWin;
    int nPlay;
    bet_function_type decide_bet;
    play_function_type decide_play;
};

Player* player_ini(bet_function_type fBet, play_function_type fPlay){
  Player *p = malloc(sizeof(Player));
  if(!fBet || !fPlay){
    fprintf(stderr, "player_ini: bet_function_type pointing NULL\n");
    return NULL;
  }

  p->hand[0] = hand_ini();
  
  if(!(p->hand[0])){
    fprintf(stderr, "player_ini: hand_ini error allocating memory\n");
    player_destroy(p);
    return NULL;
  }
  p->decide_bet = fBet;
  p->decide_play = fPlay;
  p->nTotalCards=0;
  p->cash=0;
  p->lastBet=0;
  p->lastPlayWin=true;
  p->nWin=0;
  p->nPlay=0;

  return p;
}

void player_destroy(Player *p){
  int i;
  if(!p){
    fprintf(stderr, "player_destroy: player pointing NULL\n");
    return;
  }
  for(i=0; i<MAX_HANDS; i++){
    if(p->hand[i]!=NULL){
      hand_destroy(p->hand[i]);
    }
  }
  free (p);
}

// TODO: error handling
Player *player_bet(Player *p) {
    if (!p) return NULL;
    return p->decide_bet(p);
}

Player *player_play(Player *p){
    if (!p) return NULL;
    return p->decide_play(p);
}

Player* player_addCardToHand(Player *p,int numHand,int rank){
  if(!p){
    fprintf(stderr, "player_addCardToHand: player pointing NULL\n");
    return NULL;
  }
  if(numHand<0 || numHand>MAX_HANDS){
    fprintf(stderr, "player_addCardToHand: incorrect value of numHand\n");
    return NULL;
  }
  if(rank<0 || rank>N_RANKS){
    fprintf(stderr, "player_addCardToHand: incorrect value of rank\n");
    return NULL;
  }

  hand_insertCard(p->hand[numHand], rank);
  p->nTotalCards++;

  return p;
}

Player* player_addCash(Player *p, int cash){
  if(!p){
    fprintf(stderr, "player_addCash: player pointing NULL\n");
    return NULL;
  }
  if(cash<0){
    fprintf(stderr, "player_addCash: incorrect value of cash\n");
    return NULL;
  }
  p->cash+=cash;

  return p;
}

Player* player_removeCash(Player *p, int cash){
  if(!p){
    fprintf(stderr, "player_removeCash: player pointing NULL\n");
    return NULL;
  }
  if(cash<0){
    fprintf(stderr, "player_removeCash: incorrect value of cash\n");
    return NULL;
  }
  p->cash-=cash;

  return p;
}

int player_getTotalCards(Player *p){
  if(!p){
    fprintf(stderr, "player_getTotalCards: player pointing NULL\n");
    return -1;
  }
  return p->nTotalCards;
}

int player_getCash(Player *p){
  if(!p){
    fprintf(stderr, "player_getCash: player pointing NULL\n");
    return -1;
  }
  return p->cash;
}

int player_getLastBet(Player *p){
  if(!p){
    fprintf(stderr, "player_getLastBet: player pointing NULL\n");
    return -1;
  }
  return p->lastBet;
}

bool player_lastPlayWin(Player *p){
  if(!p){
    fprintf(stderr, "player_getLastPlayWin: player pointing NULL\n");
    return false;
  }
  return p->lastPlayWin;
}

int player_getNWin(Player *p){
  if(!p){
    fprintf(stderr, "player_getNWin: player pointing NULL\n");
    return -1;
  }
  return p->nWin;
}

int player_getNPlayed(Player *p){
  if(!p){
    fprintf(stderr, "player_getNPlayed: player pointing NULL\n");
    return -1;
  }
  return p->nPlay;
}

Hand *player_getHand(Player *p, int numHand){
  if(!p){
    fprintf(stderr, "player_getHand: player pointing NULL\n");
    return NULL;
  }
  if(numHand<0 || numHand>MAX_HANDS){
    fprintf(stderr, "player_getHand: incorrect value of numHand\n");
    return NULL;
  }

  return p->hand[numHand];
}

int player_getNHands(Player*p){
    if(!p){
        return -1;
}
    return p->nTotalHands;
}