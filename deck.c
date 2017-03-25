#include "deck.h"

struct _Deck {
    int cards[N_RANKS];
    int maxSize;
    int nCurrentCards;
};

Deck* deck_ini() {
    Deck *d;
    int i;

    d = (Deck *) malloc(sizeof (Deck));
    if (!d) {
        fprintf(stderr, "hand_ini: error allocating memory.\n");
        return NULL;
    }
    for (i = 0; i < N_RANKS - 1; i++) {
        d->cards[i] = 4 * N_DECKS; /*inicializa los palos del As al 9*/
    }
    d->cards[N_RANKS] = 4 * 4 * N_DECKS; /*inicializa los palos del 10 a la K*/
    d->maxSize = 13 * 4 * N_DECKS;
    d->nCurrentCards = d->maxSize;

    return d;
}

void deck_destroy(Deck *d) {
    if (d) free(d);
}

int deck_draw(Deck *d) {
    int i, ran, aux;
    time_t t;
    if (!d) {
        fprintf(stderr, "deck_draw: deck pointing NULL.\n");
        return -1;
    }
    srand(t);
    ran = rand();
    ran %= d->nCurrentCards;

    for (i = 0, aux = d->cards[0]; i < N_RANKS; i++) {
        if (ran < aux) {
            d->cards[i]--;
            return i + 1;
        }
        aux += d->cards[i + 1];
    }

    fprintf(stderr, "deck_draw: error drawing card.\n");
    return -1;
}