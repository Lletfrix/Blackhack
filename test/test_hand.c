#include "minitest.h"

#include "../hand.h"

int main() {
    unsigned seed = time(NULL);

    printf("\nThis test run's seed is %u.\n", seed);

    printf("Testing a hand...\n");

    Hand *h;
    assert("can initialize a hand",
            (h = hand_ini()));

    assert("can add a card to the hand",
            (h = hand_insertCard(h, 9)));

    assert("hand now contains one card",
            hand_getNumCards(h) == 1);

    assert("can add one more card to the hand",
            (h = hand_insertCard(h, 8)));

    assert("hand now contains two cards",
            hand_getNumCards(h) == 2);

    int *values = hand_getValues(h);
    assert("there is only one value when there are no aces in hand",
            values[0] == 17 && values[1] == -1);

    assert("8 and 9 is a hard hand",
            hand_isHard(h));

    h = hand_insertCard(h, 1);
    free(values);
    values = hand_getValues(h);
    assert("hand has two values when there is an ace inside",
            values[0] == 28 && values[1] == 18);
    free(values);

    assert("hand is still hard because one of the values is over 21 (8 + 9 + 11)",
            hand_isHard(h));

    int *cards = hand_getCards(h);
    assert("can retrieve the cards in a hand",
            cards[0] == 1 && cards[1] == 0 && cards[2] == 0 &&
            cards[3] == 0 && cards[4] == 0 && cards[5] == 0 &&
            cards[6] == 0 && cards[7] == 1 && cards[8] == 1 &&
            cards[9] == 0);

    assert("hand gives an error when trying to retrieve a bet if none was made",
            hand_getCurrentBet(h) == E_HAND_NO_BET);

    assert("can set a hand's current bet",
            h = hand_setCurrentBet(h, 100));

    assert("can now retrieve the value of the current bet",
            hand_getCurrentBet(h) == 100);

    assert("cannot split a hand without a pair of values",
            hand_splitIsPossible(h) == false);

    h = hand_restartHand(h);
    assert("can restart a hand, clearing all cards",
            hand_getNumCards(h) == 0);

    h = hand_insertCard(h, 2);
    h = hand_insertCard(h, 2);
    assert("can split a hand with a pair of cards with the same rank",
            hand_splitIsPossible(h));

    h = hand_insertCard(h, 1);
    assert("can recognize a soft hand",
            hand_isHard(h) == false);

    Hand *crupier = hand_ini();
    crupier = hand_insertCard(crupier, 1);
    crupier = hand_insertCard(crupier, 4);
    crupier = hand_insertCard(crupier, 5);

    assert("can compare two hands",
            hand_compare(crupier, h) == LOSE);
    assert("comparing on the other order gives the oposite result",
            hand_compare(h, crupier) == WIN);
    assert("can compare and determine a tie",
            hand_compare(h, h) == TIE);

    /* hand_insertCard(crupier, 7); */
    /* assert("does not determine a tie when both hands are over 21", */
    /*         hand_compare(crupier, crupier) == LOSE); */

    crupier = hand_restartHand(crupier);
    crupier = hand_insertCard(crupier, 10);
    crupier = hand_insertCard(crupier, 8);
    h = hand_restartHand(h);
    h = hand_insertCard(h, 1);
    h = hand_insertCard(h, 4);
    h = hand_insertCard(h, 5);
    assert("can succesfully figure out which value to give an ace when comparing",
            hand_compare(crupier, h) == WIN);

    h = hand_insertCard(h, 9);
    assert("can chose the lesser value when going over 21 with an ace",
            hand_compare(crupier, h) == WIN);

    hand_destroy(h);
    assert("can destroy a deck (check that no errors ocurred)", true);

    hand_destroy(crupier);

    return 0;
}
