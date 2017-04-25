#include "../play_strategies.h"



Player *play_basic(Player* p, Table* t){
    Deck *deck;
    Hand *playerHand, *crupierHand;
    int *pHandValues, *cHandValues, cCard, numHands=1, currentHand=0;
    bool y_n_doubled=false, y_n_stand=false;
    if(!p||!t){
        fprintf(stderr, "%s\n", "play_basic: invalid arguments.");
        return NULL;
    }

    deck=table_getDeck(t);
    if(!deck){
        fprintf(stderr, "%s\n", "play_basic: error getting deck");
        return NULL;
    }

    /*Suponiendo que el jugador ya tiene las dos primeras cartas*/

    playerHand=player_getHand(p, 0);
    crupierHand=crupier_getHand(c);

    pHandValues=hand_getValues(playerHand);
    cHandValues=hand_getValues(crupierHand);
    cCard=cHandValues[0];
    free(cHandValues);

    while(currentHand<numHands){
        /* Soft Hand */
        y_n_doubled=false;
        y_n_stand=false;

        if(pHandValues[1]!=-1){
            switch (pHandValues[0]) {
                case 3:
                case 4:
                    if(cCard==5||cCard==6){
                        /* Double */
                        player_removeCash(p, player_getLastBet);
                        player_addCardToHand(p, currentHand, deck_draw(deck));
                        y_n_doubled=true;
                    }
                    else{
                        /* Hit */
                        player_addCardToHand(p, currentHand, deck_draw(deck));
                    }
                    break;
            }
        }

        /* When you get over 21, you doubled or you decide to stand you are done with your hand */
        if (((pHandValues[0]>21) && (pHandValues[1]==-1)) || (pHandValues[1]>21) || (y_n_doubled==true) || (y_n_stand==true)) {
            currentHand++;
        }
    }

}
