#include "crupier.h"

struct _Crupier{
    Hand* hand;
};

Crupier *crupier_ini(){
  Crupier *c;
  c->hand=hand_ini();
  if(!(c->hand)){
    fprintf(stderr, "crupier_ini: hand_ini error allocating memory\n");
    crupier_destroy(c);
    return NULL;
  }
  return c;
}

void crupier_destroy(Crupier *c){
  if(!c){
    fprintf(stderr, "crupier_destroy: crupier pointing NULL\n");
    return;
  }

  if(c->hand!=NULL){
    hand_destroy(c->hand);
  }

  free (c);
}

Crupier* crupier_play (Crupier *c, Deck *d){
  int *handValues;
  if(!c){
    fprintf(stderr, "crupier_play: crupier pointing NULL\n");
    return;
  }
  /*Suponiendo que el crupier ya tiene la primera carta*/

  /*Pide otra carta*/
  c=crupier_addCard(c, d);
  handValues=hand_getValues(crupier_getHand(c));
  if(!handValues){
    fprintf(stderr, "crupier_play: handValues pointing NULL\n");
    return NULL;
  }
  while(handValues[0]<17){
    c=crupier_addCard(c, d);
  }
  if(handValues[0]>21){
    if(handValues[1]>0){
      while(handValues[1]<17){
        c=crupier_addCard(c, d);
      }
      return c;
    }
  }
  else{
    return c;
  }
}

Crupier* crupier_addCard(Crupier *c, int rank){
  if(!c){
    fprintf(stderr, "crupier_addCard: crupier pointing NULL\n");
    return NULL;
  }
  if(rank<0 || rank>N_RANKS){
    fprintf(stderr, "crupier_addCard: incorrect value of rank\n");
    return NULL;
  }

  hand_insertCard(c->hand, rank);

  return c;
}

Hand* crupier_getHand(Crupier *c){
  if(!c){
    fprintf(stderr, "crupier_getHand: crupier pointing NULL\n");
    return NULL;
  }

  return c->hand;
}
