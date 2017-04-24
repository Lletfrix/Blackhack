#include "crupier.h"

struct _Crupier {
    Hand* hand;
};

Crupier *crupier_ini() {
    Crupier *c = malloc(sizeof (Crupier));
    if (!c) {
        fprintf(stderr, "crupier_ini: malloc error allocating memory\n");
        return NULL;
    }

    c->hand = hand_ini();
    if (!(c->hand)) {
        fprintf(stderr, "crupier_ini: hand_ini error allocating memory\n");
        free(c);
        return NULL;
    }
    return c;
}

void crupier_destroy(Crupier *c) {
    if (!c) {
        fprintf(stderr, "crupier_destroy: crupier pointing NULL\n");
        return;
    }

    if (c->hand != NULL) {
        hand_destroy(c->hand);
    }

    free(c);
}

Crupier* crupier_play(Crupier *c, Deck *d) {
    int *handValues;
    if (!c) {
        fprintf(stderr, "crupier_play: crupier pointing NULL\n");
        return NULL;
    }
    /*Suponiendo que el crupier ya tiene la primera carta*/

    /*Pide otra carta*/
    c = crupier_addCard(c, deck_draw(d));
    handValues = hand_getValues(crupier_getHand(c));
    if (!handValues) {
        fprintf(stderr, "crupier_play: handValues pointing NULL\n");
        return NULL;
    }
    /*Añade cartas a la mano mientras el primer valor(As=11) sea menor que 17*/
    while (handValues[0] < 17) {
        c = crupier_addCard(c, deck_draw(d));
        free(handValues);
        handValues = hand_getValues(crupier_getHand(c));
        if (!handValues) {
            fprintf(stderr, "crupier_play: handValues pointing NULL\n");
            return NULL;
        }
    }
    /*Si no sobrepasa 21 devuelve c, si se pasa, comprueba que existe el segundo
     valor, (As=1) y pide cartas igual que antes*/
    if (handValues[0] > 21) {
        if (handValues[1] > 0) {
            while (handValues[1] < 17) {
                c = crupier_addCard(c, deck_draw(d));
                free(handValues);
                handValues = hand_getValues(crupier_getHand(c));
                if (!handValues) {
                    fprintf(stderr, "crupier_play: handValues pointing NULL\n");
                    return NULL;
                }
            }
            free(handValues);
            return c;
        }
    }
    free(handValues);
    return c;
}

Crupier* crupier_addCard(Crupier *c, int rank) {
    if (!c) {
        fprintf(stderr, "crupier_addCard: crupier pointing NULL\n");
        return NULL;
    }
    if (rank < 0 || rank > N_RANKS) {
        fprintf(stderr, "crupier_addCard: incorrect value of rank\n");
        return NULL;
    }

    hand_insertCard(c->hand, rank);

    return c;
}

Hand* crupier_getHand(Crupier *c) {
    if (!c) {
        fprintf(stderr, "crupier_getHand: crupier pointing NULL\n");
        return NULL;
    }

    return c->hand;
}

void crupier_print(FILE*fp, Crupier*c){
    if(!fp || !c){
        return;
    }
    fprintf(fp, "\tCrupier hand: ");
    hand_print(fp, c->hand);
}
