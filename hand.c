#include "hand.h"

struct _Hand {
    int cards[10];
    int nCurrentCards;
};

Hand* hand_ini() {
    Hand* h = NULL;
    int i;
    h = (Hand*) malloc(sizeof (Hand));
    if (!h) {
        fprintf(stderr, "hand_ini: error allocating memory.\n");
        return NULL;
    }
    for (i = 0; i < 10; i++) {
        h->cards[i] = 0;
    }
    h->nCurrentCards = 0;
    return h;
}

void hand_destroy(Hand* h) {
    if (!h) {
        fprintf(stderr, "hand_destroy: hand pointing NULL.\n");
        return;
    }
    free(h);
}

Hand* hand_insertCard(Hand* h, int rank) {
    if (!h || rank < 1 || rank > 10) {
        fprintf(stderr, "hand_insertCard: invalid arguments.\n");
        return NULL;
    }
    h->cards[rank - 1]++;
    return h;
}

int* hand_getValues(Hand*h) {
    int* values, i;
    if (!h) {
        fprintf(stderr, "hand_getValues: invalid argument.\n");
        return NULL;
    }
    if (*h->cards > 0 && *h->cards < 12) {
        values = (int*) malloc(2 * sizeof (int));

        if (!values) {
            fprintf(stderr, "hand_getValues: error allocating memory.\n");
            return NULL;
        }
        *values = *h->cards;
        values[1] = *h->cards + 10;

        for (i = 1; i < N_RANKS; i++) {
            *values += h->cards[i];
            values[1] += h->cards[i];
        }
        return values;

    }

    values = (int*) sizeof (int);
    if (!values) {
        fprintf(stderr, "hand_getValues: error allocating memory.\n");
        return NULL;
    }

    for (i = 0, *values = 0; i < N_RANKS; i++) {
        *values += h->cards[i];
    }
    return values;
}

Hand* hand_divide(Hand* original) {
    Hand* new = NULL;
    int i, aux = -1;
    if (!original) {
        fprintf(stderr, "hand_divide: invalid argument.\n");
        return NULL;
    }
    new = hand_ini();
    if (!new) {
        fprintf(stderr, "hand_divide: error in hand_ini.\n");
        return NULL;
    }
    for (i = 0; i < N_RANKS; i++) {
        if (original->cards[i] == 2) {
            aux = i;
            break;
        }
    }
    if (aux == -1) {
        fprintf(stderr, "hand_divide: no double card found.\n");
        return NULL;
    }
    original->cards[aux]--;
    new->cards[aux]++;
    return new;
}

int hand_getNumCards(Hand* h) {
    if (!h) {
        fprintf(stderr, "hand_getNumCards: invalid arguments.\n");
        return -1;
    }
    return h->nCurrentCards;
}

int* hand_getCards(Hand*h) {
    if (!h) {
        fprintf(stderr, "hand_getCards: invalid arguments.\n");
        return NULL;
    }
    return h->cards;
}