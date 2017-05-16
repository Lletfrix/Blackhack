#include "hand.h"

struct _Hand {
    int cards[10];
    int nCurrentCards;
    int currentBet;
    int lastBet;
    bool hardHand;
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
    int* values;
    if (!h || rank < 1 || rank > 10) {
        fprintf(stderr, "hand_insertCard: invalid arguments.\n");
        return NULL;
    }
    h->cards[rank - 1]++;
    h->nCurrentCards++;

    /* Actualización de softHand/hardHand */
    values=hand_getValues(h);

    if(values[1]<0){
        h->hardHand=true;
    }
    else{
        if(values[0]>21){
            h->hardHand=true;
        }
        else{
            h->hardHand=false;
        }
    }

    free (values);


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
            *values += (i+1)*h->cards[i];
            values[1] += (i+1)*h->cards[i];
        }

        return values;

    }

    for (i = 0, *values = 0; i < N_RANKS; i++) {
        *values += (i+1)*h->cards[i];
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
    if (aux == 0){
        original->hardHand=false;
        new->hardHand=false;
    }
    original->cards[aux]--;
    original->nCurrentCards--;
    new->cards[aux]++;
    new->nCurrentCards++;
    return new;
}

Hand* hand_restartHand (Hand* h){
    int i;
    if(!h){
        fprintf(stderr, "hand_restartHand: invalid arguments");
        return NULL;
    }
    for(i=0;i<N_RANKS;i++){
        h->cards[i]=0;
    }
    h->nCurrentCards=0;
    h->lastBet=h->currentBet;
    h->currentBet=0;
    h->hardHand=true;
    return h;
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

bool hand_isHard(Hand* h){
    return h->hardHand;
}

Peg hand_compare(Hand *crupier_hand, Hand *player_hand){
    int *cValues, *hValues, aux=0, aux2=0;
    bool crup_hard, play_hard;
    cValues = hand_getValues(crupier_hand);
    hValues = hand_getValues(player_hand);
    crup_hard = hand_isHard(crupier_hand);
    play_hard = hand_isHard(player_hand);

    if (play_hard == false){ /* Soft hand, 0 position of array will always be 22>x>0, As = 11 */
        if (crup_hard == false) {

            if (hValues[0]>21) return LOSE;
            else if (cValues[0]>21) return WIN;
            else if(hValues[0]>cValues[0]) return WIN;
            else if (hValues[0] == cValues[0]) return TIE;
            else if (hValues[0] < cValues[0]) return LOSE;
            else{
                fprintf(stderr, "%s\n", "hand_compare:fatal error");
                return LOSE;
            }
        }
        else if (crup_hard==true){
            if (cValues[0] > 21 && cValues[0]!=-1) aux = cValues[1];
            else aux = cValues[0];
            /**********************************************************/
            if (hValues[0]>21) return LOSE;
            else if (aux>21) return WIN;
            else if(hValues[0]>aux) return WIN;
            else if (hValues[0] == aux) return TIE;
            else if (hValues[0] < aux) return LOSE;
            else{
                fprintf(stderr, "%s\n", "hand_compare:fatal error");
                return LOSE;
            }
        }
    }
    else if (play_hard==true){ /* Hard hand, 2nd value will only be used if the first one is above 21 and the second one exists */
        if (hValues[0]>21 && hValues[0]!=-1) aux2 = hValues[1];
        else aux2 = hValues[0];
        /************************************************************/
        if (crup_hard == false) {

            if (aux2>21) return LOSE;
            else if (cValues[0]>21) return WIN;
            else if(aux2>cValues[0]) return WIN;
            else if (aux2 == cValues[0]) return TIE;
            else if (aux2 < cValues[0]) return LOSE;
            else{
                fprintf(stderr, "%s\n", "hand_compare:fatal error");
                return LOSE;
            }
        }
        else if (crup_hard==true){
            if (cValues[0] > 21 && cValues[0]!=-1) aux = cValues[1];
            else aux = cValues[0];
            /**********************************************************/
            if (aux2 > 21) return LOSE;
            else if (aux > 21) return WIN;
            else if(aux2 > aux) return WIN;
            else if (aux2 == aux) return TIE;
            else if (aux2 < aux) return LOSE;
            else{
                fprintf(stderr, "%s\n", "hand_compare:fatal error");
                return LOSE;
            }
        }
    }else{
        fprintf(stderr, "%s\n", "hand_compare:fatal error");
        return LOSE;
    }
    return LOSE;
}

int hand_getCurrentBet(Hand* h){
    if(!h) return -1;
    return h->currentBet;
}

Hand* hand_setCurrentBet (Hand* h, int bet){
    if (!h || bet < 0) return NULL;
    h->currentBet=bet;
    return h;
}

bool hand_splitIsPossible(Hand* h){
    if(!h){
        fprintf(stderr, "%s\n", "Hand pointing NULL");
        return false;
    }
    if (h->nCurrentCards!=2){
        return false;
    }
    for (size_t i = 0; i < N_RANKS; i++) {
        if(h->cards[i]==2){
            return true;
        }
    }
    return false;
}

void hand_print(FILE* fp, Hand* h){
    int *values;
    if(!fp || !h){
        return;
    }
    values = hand_getValues(h);
    fprintf(fp, "\tCurrentCards: %d, Values[%d/%d]: [",h->nCurrentCards,values[0],values[1]);
    for (size_t i = 0; i < N_RANKS; i++) {
        fprintf(fp, " %d,",h->cards[i]);
    }
    fprintf(fp, "]\n");
    free(values);
}
