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

    values = (int*) malloc(2 * sizeof (int));

    if (!values) {
        fprintf(stderr, "hand_getValues: error allocating memory.\n");
        return NULL;
    }
    
    if (*h->cards > 0 && *h->cards < 12) {
        *values = *h->cards + 10;
        values[1] = *h->cards;

        for (i = 1; i < N_RANKS; i++) {
            *values += i*h->cards[i];
            values[1] += i*h->cards[i];
        }
        
        return values;

    }

    for (i = 0, *values = 0; i < N_RANKS; i++) {
        *values += i*h->cards[i];
    }
    values[1] = -1;

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

hand_compare_result hand_compare(Hand *crupier_hand, Hand *player_hand)
{
    if (!crupier_hand || !player_hand) {
        fprintf(stderr, "hand_compare: invalid arguments.\n");
        return compare_error;
    }

    int *crupier_hand_values = hand_getValues(crupier_hand);
    int *player_hand_values = hand_getValues(player_hand);

    // si el jugador se ha pasado y no tiene segundo valor, gana el crupier
    if (player_hand_values[0] > 21 && player_hand_values[1] < 0) {
        return crupier_wins;
    }
    // ahora utilizamos el segundo valor de la mano del jugador porque el primero se ha pasado
    else if (player_hand_values[0] > 21 && player_hand_values[1] > 0) {
        // comprobar con los dos valores posibles de la mano del crupier

        // comprobamos si el crupier se pasa con el primer valor y no existe el segundo valor
        if (crupier_hand_values[0] > 21 && crupier_hand_values[1] < 0) {
            return player_wins;
        }
        // ahora si el crupier se pasa pero sí existe el segundo valor
        else if (crupier_hand_values[0] > 21 && crupier_hand_values[1] > 0) {
            // comparar los segundos valores de las dos manos
            if (player_hand_values[1] > crupier_hand_values[1])
                return player_wins;
            else if (player_hand_values[1] < crupier_hand_values[1])
                return crupier_wins;
            else
                return tie;
        }
        // si el crupier no se pasa con el primer valor, lo utilizamos
        else {
            if (player_hand_values[1] > crupier_hand_values[0])
                return player_wins;
            else if (player_hand_values[1] < crupier_hand_values[0])
                return crupier_wins;
            else
                return tie;
        }
    }
    // el jugador no se ha pasado con el primer valor, ignoramos el segundo
    else {
        if (crupier_hand_values[0] > 21 && crupier_hand_values[1] < 0) {
            return player_wins;
        }
        // ahora si el crupier se pasa pero sí existe el segundo
        else if (crupier_hand_values[0] > 21 && crupier_hand_values[1] > 0) {
            // comparar los segundos valores de las dos manos
            if (player_hand_values[0] > crupier_hand_values[1])
                return player_wins;
            else if (player_hand_values[0] < crupier_hand_values[1])
                return crupier_wins;
            else
                return tie;
        }
        // comparamos el primer valor de la mano del jugador con el primer valor de la mano del crupier
        else {
            if (player_hand_values[0] > crupier_hand_values[0])
                return player_wins;
            else if (player_hand_values[0] < crupier_hand_values[0])
                return crupier_wins;
            else
                return tie;
        }
    }
}