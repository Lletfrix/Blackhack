#include "../play_strategies.h"

char recommendation_17S_DAS(int player_hand, int crupier_hand) {

    /*
     * x is for hit, D is for double-down, S is for split, a is for early surrender,
     * - is for stand.
     *
     *   To acces this matrix:
     *   number_strategy[PLAYER_CONVERT(player_hand)][CRUPIER_CONVERT(crupier_hand)]
     */

    char number_strategy[17][10] = {
    //    2    3    4    5    6    7    8    9    10   A
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'a'}, // 5
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'a'}, // 6
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'a'}, // 7
        {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'}, // 8
        {'x', 'D', 'D', 'D', 'D', 'x', 'x', 'x', 'x', 'x'}, // 9
        {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'x', 'x'}, // 10
        {'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'x'}, // 11
        {'x', 'x', '-', '-', '-', 'x', 'x', 'x', 'x', 'a'}, // 12
        {'-', '-', '-', '-', '-', 'x', 'x', 'x', 'x', 'a'}, // 13
        {'-', '-', '-', '-', '-', 'x', 'x', 'x', 'a', 'a'}, // 14
        {'-', '-', '-', '-', '-', 'x', 'x', 'x', 'a', 'a'}, // 15
        {'-', '-', '-', '-', '-', 'x', 'x', 'a', 'a', 'a'}, // 16
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}, // 17
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', 'a'}, // 18
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', 'a'}, // 19
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', 'a'}, // 20
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', 'a'}, // 21
    };

    if (player_hand > 21 || player_hand < 5) {
        fprintf(stderr, "strategy can't recommend anything for that value (%d)\n", player_hand);
        return 'E';
    }

    return number_strategy[player_hand - 5][crupier_hand - 2];
}

Player *play_basic_17S_DAS_matrix(Player* p, Table* t){

    int *player_hand, *crupier_hand, player_value;
    char decision;
    Deck *d;
    Hand *current_hand;

    if (!p || !t) {
        fprintf(stderr, "play_basic_17S_DAS_matrix: invalid parameters\n");
        return NULL;
    }

    d = table_getDeck(t);

    crupier_hand = hand_getValues(crupier_getHand(table_getCrupier(t)));
    if (!crupier_hand) {
        fprintf(stderr, "play_strategy: could not get crupier hand\n");
        return NULL;
    }

    while (1) {
        player_hand = hand_getValues(player_getHand(p, 0));
        if (!player_hand) {
            fprintf(stderr, "play_strategy: handValues pointing NULL\n");
            return NULL;
        }

        // chose what value of the hand we'll use for this iteration of the strategy
        player_value = (player_hand[0] > 21 && player_hand[1] != -1) ? player_hand[1] : player_hand[0];

        // Blackjack! Pay immediately;
        if (player_value == 21){
            player_addCash(p, 2.5*hand_getCurrentBet(player_getHand(p, 0)));
            return p;
        }

        decision = recommendation_17S_DAS(player_value, crupier_hand[0]);
        printf("%d vs %d: player choses to %c\n", player_value, crupier_hand[0], decision);
        switch(decision) {
            // hit, ask for another card
            case 'x':
                p = player_addCardToHand(p, 0, deck_draw(d));
                break;
            case 's':
                printf("splitting is not implemented at the moment\n");
                return p;
            case 'D': // double
                current_hand = player_getHand(p, 0);
                p = player_addCardToHand(p, 0, deck_draw(d));
                int current_bet = hand_getCurrentBet(current_hand);
                p = player_removeCash(p, current_bet);
                hand_setCurrentBet(current_hand, current_bet * 2);
                return p;
            default: // stay
                return p;
        }


    }

    return p;
}
