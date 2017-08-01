#include "deck.h"

struct _Deck {
    int cards[N_RANKS];
    int maxSize;
    int nCurrentCards;
    float running_count;
    unsigned seed;
};

Deck *deck_updateRunningCount(Deck *d, int rank);

Deck* deck_ini(unsigned seed) {
    Deck *d;
    int i;

    /*  TODO: replantearse no hacer el caso de seed vacío aquí */
    d = (Deck *) malloc(sizeof (Deck));

    if (!d) {
        fprintf(stderr, "deck_ini: error allocating memory.\n");
        return NULL;
    }

    for (i = 0; i < N_RANKS - 1; i++) {
        d->cards[i] = 4 * N_DECKS; /*inicializa los palos del As al 9*/
    }
    d->cards[N_RANKS - 1] = 4 * 4 * N_DECKS; /*inicializa los palos del 10 a la K*/
    d->nCurrentCards = N_CARDS_DECK * N_DECKS;
    d->seed = seed;
    d->running_count = 0;

    return d;
}

void deck_destroy(Deck *d) {
    free(d);
}

int deck_draw(Deck *d) {
    int i, ran, aux;

    if (!d) {
        fprintf(stderr, "deck_draw: deck pointing NULL.\n");
        return -1;
    }
    if(d->nCurrentCards==0){
        fprintf(stderr, "deck_draw: deck is empty.\n");
        return -1;
    }

    // TODO: fix seeding
    ran = rand();
    ran %= d->nCurrentCards;

    for (i = 0, aux = d->cards[0]; i < N_RANKS; i++) {
        if (ran < aux) {
            d->cards[i]--;
            d->nCurrentCards--;
            d = deck_updateRunningCount(d, i + 1);
            return i + 1;
        }
        aux += d->cards[i + 1];
    }

    fprintf(stderr, "deck_draw: error drawing card.\n");
    return -1;
}

bool deck_isEmpty (Deck* d){
    if (!d) {
        fprintf(stderr, "deck_isEmpty: deck pointing NULL.\n");
        return false;
    }
    if(d->nCurrentCards==0){
        fprintf(stderr, "deck_isEmpty: deck is empty.\n");
        return true;
    }
    return false;
}

int deck_getNCurrentCards(Deck* d){
    if (!d){
        fprintf(stderr, "deck_getNCurrentCards: deck pointing NULL");
        return -1;
    }
    return d->nCurrentCards;
}

Deck* deck_restartDeck(Deck *d){
  if(!d){
    fprintf(stderr, "deck_restartDeck: deck pointing NULL");
    return NULL;
  }
  for (int i = 0; i < N_RANKS - 1; i++) {
      d->cards[i] = 4 * N_DECKS; /*inicializa los palos del As al 9*/
  }
  d->cards[N_RANKS - 1] = 4 * 4 * N_DECKS; /*inicializa los palos del 10 a la K*/
  d->maxSize = 13 * 4 * N_DECKS;
  d->nCurrentCards = d->maxSize;
  d->seed = time(NULL)*clock();
  d->running_count = 0;
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

    return deck_updateRunningCount(d, rank);
}

int deck_getNCardsofRank(Deck* d,  int rank){
    if(!d || rank>10 || rank<1 ){
        return -1;
    }
    return d->cards[rank-1];
}

float deck_runningCount(Deck *d) {
    if (!d) {
        fprintf(stderr, "deck_runningCount: invalid deck given\n");
        return E_INVALID_DECK;
    }

    return d->running_count;
}

float deck_trueCount(Deck *d) {
    if (!d) {
        fprintf(stderr, "deck_runningCount: invalid deck given\n");
        return E_INVALID_DECK;
    }

    return d->running_count / d->nCurrentCards * N_CARDS_DECK;
}

Deck *deck_updateRunningCount(Deck *d, int drawn_card) {
    // TODO: maybe make this a modifiable property of Deck
    float count_strategy[] = {
    //  A    2   3   4   5   6  7  8  9   T
        -4, +2, +3, +3, +4, +3, 2, 0, -1, -3
    };

    if (drawn_card < 1 || drawn_card > 10) {
        fprintf(stderr, "deck_updateRunningCount: invalid card drawn given\n");
        return NULL;
    }

    d->running_count += count_strategy[drawn_card - 1];
    return d;
}
