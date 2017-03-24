#include "deck.h"
#include <stdlib.h>
#include <stdio.h>

struct _Deck {
    int cards[N_RANK];
    int size;
    int nCurrentCards;
};

Deck* deck_ini(unsigned int N_DECKS){
    Deck *d;
    int i;

    d=(Deck *)malloc(sizeof(Deck));
    if(!d){
      fprintf(stderr, "hand_ini: error allocating memory");
      return NULL;
    }
    for(i=0;i<N_RANKS-1;i++){
      cards[i]=4*N_DECKS; /*inicializa los palos del As al 9*/
    }
    cards[N_RANKS]=4*4*N_DECKS; /*inicializa los palos del 10 a la K*/
    size=13*4*N_DECKS;
    nCurrentCards=size;

    return d;
}

void deck_destroy(Deck *d){
  if(d) free(d);
}

int deck_draw(Deck *d){
  if(!d){
    fprintf(stderr, "deck_draw: deck pointing NULL");
    return NULL;
  }
  /*
  1º funcion que devuelve aleatoriamente el valor de la carta a sacar
     en funcion de las cartas que quedan
  2º reducir en 1 el valor de la carta en el array
  3º devolver el valor de la carta
  */
}
