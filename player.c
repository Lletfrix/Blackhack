
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
    int nTie;
    int nPlay;
    bet_function_type decide_bet;
    play_function_type decide_play;
};

Player* player_ini(bet_function_type fBet, play_function_type fPlay){
  Player *p = malloc(sizeof(Player));
  if(!p){
    fprintf(stderr, "player_ini: malloc error allocating memory\n");
    return NULL;
  }
  if(!fBet || !fPlay){
    fprintf(stderr, "player_ini: bet_function_type pointing NULL\n");
    return NULL;
  }

  p->hand[0] = hand_ini();

  if(!(p->hand[0])){
    fprintf(stderr, "player_ini: hand_ini error allocating memory\n");
    free(p);
    return NULL;
  }
  p->decide_bet = fBet;
  p->decide_play = fPlay;
  p->nTotalHands=1;
  p->nTotalCards=0;
  p->cash=0;
  p->lastBet=0;
  p->lastPlayWin=true;
  p->nWin=0;
  p->nTie=0;
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

Player *player_bet(Player *p, Table *t) {
	if (!p || !t) {
	fprintf(stderr, "player_bet: invalid arguments.\n");
		return NULL;
		}
	return p->decide_bet(p, t);
}

Player *player_play(Player *p, Table *t){
    if (!p || !t){
        fprintf(stderr, "player_play: invalid arguments.\n");
        return NULL; }
    return p->decide_play(p, t);
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

Player* player_addGame (Player* p, int cmpValue){

    if(!p || cmpValue<0 || cmpValue>2){
        fprintf(stderr,"player_addGame: invalid arguments.\n");
        return NULL;
    }
    p->nPlay++;
    if(cmpValue==1){
        p->nTie++;
    }
    if(cmpValue==2){
        p->nWin++;
    }
    return p;
}


Player* player_restartHands (Player*p){
    int i;
    if(!p){
        fprintf(stderr, "player_restarHands: invalid arguments.\n");
        return NULL;
    }
    for (i=0;i<p->nTotalHands;i++){
        p->hand[i]=hand_restartHand(p->hand[i]);
        if (!p->hand[i]){
            fprintf(stderr, "player_restartHands: error restarting hand %d", i);
            return NULL;
        }
    }
    p->nTotalHands=1;
    p->nTotalCards=0;
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

int player_getNTie(Player *p){
  if(!p){
    fprintf(stderr, "player_getNTie: player pointing NULL\n");
    return -1;
  }
  return p->nTie;
}

int player_getNLost(Player *p){
  if(!p){
    fprintf(stderr, "player_getNLost: player pointing NULL\n");
    return -1;
  }
  return p->nPlay-(p->nWin+p->nTie);
}

int player_getNPlayed(Player *p){
  if(!p){
    fprintf(stderr, "player_getNPlayed: player pointing NULL\n");
    return -1;
  }
  return p->nPlay;
}

double player_getWinRatio(Player *p){
    double nwin, nplayed, ratio;
  if(!p){
    fprintf(stderr, "player_getWinRatio: player pointing NULL\n");
    return -1;
  }
  nwin=(double)player_getNWin(p);
  nplayed=(double)player_getNPlayed(p);
  ratio=(nwin/nplayed);
  return ratio*100.0;
}

double player_getTieRatio(Player *p){
    double ntie, nplayed, ratio;
  if(!p){
    fprintf(stderr, "player_getTieRatio: player pointing NULL\n");
    return -1;
  }
  ntie=(double)player_getNTie(p);
  nplayed=(double)player_getNPlayed(p);
  ratio=(ntie/nplayed);
  return ratio*100.0;
}

double player_getLostRatio(Player *p){
    double nlost, nplayed, ratio;
    if(!p){
    fprintf(stderr, "player_getLostRatio: player pointing NULL\n");
    return -1;
    }
    nlost=(double)player_getNLost(p);
    nplayed=(double)player_getNPlayed(p);
    ratio=(nlost/nplayed);
    return ratio*100.0;
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
