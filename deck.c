#include "deck.h"


struct _Deck {
    int cards[N_RANKS];
    int size;
    int nCurrentCards;
};

Deck* deck_ini(){
    Deck *d;
    int i;

    d=(Deck *)malloc(sizeof(Deck));
    if(!d){
      fprintf(stderr, "hand_ini: error allocating memory");
      return NULL;
    }
    for(i=0;i<N_RANKS-1;i++){
      d->cards[i]=4*N_DECKS; /*inicializa los palos del As al 9*/
    }
    d->cards[N_RANKS]=4*4*N_DECKS; /*inicializa los palos del 10 a la K*/
    d->size=13*4*N_DECKS;
    d->nCurrentCards=d->size;

    return d;
}

void deck_destroy(Deck *d){
  if(d) free(d);
}

int deck_draw(Deck *d){
  if(!d){
    fprintf(stderr, "deck_draw: deck pointing NULL");
    return -1;
  }
  /*
  1º funcion que devuelve aleatoriamente el valor de la carta a sacar
     en funcion de las cartas que quedan
  2º reducir en 1 el valor de la carta en el array
  3º devolver el valor de la carta
  */
}
