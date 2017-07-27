#include "deck.h"

struct _Deck {
    int cards[N_RANKS];
    int maxSize;
    int nCurrentCards;
    unsigned seed;
};

Deck* deck_ini(unsigned seed) {
    Deck *d;
    int i;

    /*  TODO: replantearse no hacer el caso de seed vacío aquí */
    d = (Deck *) malloc(sizeof (Deck));

    if (!d) {
        fprintf(stderr, "hand_ini: error allocating memory.\n");
        return NULL;
    }

    for (i = 0; i < N_RANKS - 1; i++) {
        d->cards[i] = 4 * N_DECKS; /*inicializa los palos del As al 9*/
    }
    d->cards[N_RANKS - 1] = 4 * 4 * N_DECKS; /*inicializa los palos del 10 a la K*/
    d->maxSize = 13 * 4 * N_DECKS;
    d->nCurrentCards = d->maxSize;
    d->seed = seed;

    return d;
}

void deck_destroy(Deck *d) {
    free(d);
}

int deck_draw(Deck *d) {
    int i, ran, aux;

#ifdef DEBUG
    if (!d) {
        fprintf(stderr, "deck_draw: deck pointing NULL.\n");
        return -1;
    }
    if(d->nCurrentCards==0){
        fprintf(stderr, "deck_draw: deck is empty.\n");
        return -1;
    }
#endif
    // TODO: fix seeding
    ran = rand();
    ran %= d->nCurrentCards;

    for (i = 0, aux = d->cards[0]; i < N_RANKS; i++) {
        if (ran < aux) {
            d->cards[i]--;
            d->nCurrentCards--;
            return i + 1;
        }
        aux += d->cards[i + 1];
    }

    fprintf(stderr, "deck_draw: error drawing card.\n");
    return -1;
}

bool deck_isEmpty (Deck* d){
#ifdef DEBUG
    if (!d) {
        fprintf(stderr, "deck_isEmpty: deck pointing NULL.\n");
        return false;
    }
#endif
    if(d->nCurrentCards==0){
        fprintf(stderr, "deck_isEmpty: deck is empty.\n");
        return true;
    }
    return false;
}

int deck_getNCurrentCards(Deck* d){
#ifdef DEBUG
    if (!d){
        fprintf(stderr, "deck_getNCurrentCards: deck pointing NULL");
        return -1;
    }
#endif
    return d->nCurrentCards;
}

Deck* deck_restartDeck(Deck *d){
#ifdef DEBUG
  if(!d){
    fprintf(stderr, "deck_restartDeck: deck pointing NULL");
    return NULL;
  }
#endif
  for (int i = 0; i < N_RANKS - 1; i++) {
      d->cards[i] = 4 * N_DECKS; /*inicializa los palos del As al 9*/
  }
  d->cards[N_RANKS - 1] = 4 * 4 * N_DECKS; /*inicializa los palos del 10 a la K*/
  d->maxSize = 13 * 4 * N_DECKS;
  d->nCurrentCards = d->maxSize;
  d->seed = time(NULL)*clock();
  srand(d->seed);
  return d;
}

void deck_print(FILE* pf, Deck* d){
    if(!pf||!d){
        return;
    }
    fprintf(pf, "\tCurrent [%d]: [", d->nCurrentCards);
    for (size_t i = 0; i < N_RANKS; i++) {
        fprintf(pf, " %d,", d->cards[i]);
    }
    fprintf(pf, "]\n");
}

double *deck_distribution(Deck *d)
{
    double *probs = malloc(sizeof(double) * N_RANKS);
    double cards = (double) deck_getNCurrentCards(d);
    if (!probs) return NULL;

    for (int i = 0; i < N_RANKS; i++) {
        probs[i] = deck_getNCardsofRank(d, i + 1) / cards;
    }

    return probs;
}


Deck *deck_removeCard(Deck *d, int rank)
{
    if (!d || rank < 1 || rank > 10) {
        fprintf(stderr, "deck_removeCard: invalid deck or rank given\n");
        return NULL;
    }

    if (d->cards[rank - 1] < 1) {
        fprintf(stderr, "deck_removeCard: no cards of that rank left in deck\n");
    }

    d->cards[rank - 1]--;

    return d;
}

int deck_getNCardsofRank(Deck* d,  int rank){
    if(!d || rank>10 || rank<1 ){
        return -1;
    }
    return d->cards[rank-1];
}
