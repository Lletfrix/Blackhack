#include "../play_strategies.h"

/* Funciones internas de estrategia, acciones del jugador*/
bool strategy_split(Player* player, int currentHand, Deck* deck){
    int index, currentBet;
    index=player_splitHand(player, currentHand);
    if(index==-1){
        return false;
    }
    if(NULL==player_addCardToHand(player, currentHand, deck_draw(deck))){
        return false;
    }
    if(NULL==player_addCardToHand(player, index, deck_draw(deck))){
        return false;
    }
    currentBet = hand_getCurrentBet(player_getHand(player, currentHand));
    player_removeCash(player, currentBet);
    hand_setCurrentBet(player_getHand(player, index), currentBet);
    return true;
}

Status strategy_hit(Player* player, int currentHand, Hand* hand, Deck* deck){
    if (hand != player_getHand(player, currentHand)){
        fprintf(stderr, "%s\n", "ERROR FATAL");
        return FAIL;
    }
    if(NULL==player_addCardToHand(player, currentHand, deck_draw(deck))){
        return FAIL;
    }
    return OK;
}

bool strategy_double(Player* player, int currentHand, Hand* hand, Deck* deck){
    int currentBet, aux;
    if (hand != player_getHand(player, currentHand)){
        fprintf(stderr, "%s\n", "ERROR FATAL");
        return false;
    }
    if(NULL==player_addCardToHand(player, currentHand, deck_draw(deck))){
        return false;
    }
    currentBet=hand_getCurrentBet(player_getHand(player, currentHand));
    aux=hand_getCurrentBet(hand);
    if (currentBet != aux){
        fprintf(stderr, "%s\n", "ERROR FATAL");
        return false;
    }
    player_removeCash(player, currentBet);
    currentBet*=2;
    hand_setCurrentBet(hand, currentBet);
    return true;;
}


Player *play_basic_17S_DAS(Player* player, Table* table){
    /* we are going to use the hand being played, the player, the crupier, the deck, the crupier hand */
    Deck *deck=NULL;
    Crupier *crupier=NULL;
    Hand *playerHand=NULL, *crupierHand=NULL;
    int *playerHand_values=NULL, *crupierHand_values=NULL;
    int crupierCard=-1, currentHand=0, numHands=1, playerPoints=-1;
    Status code;
    /* flags */
    bool split_once=false, split_current=false, split_possible=false, stop=false, hard_hand=true;

    if(!player || !table){
        fprintf(stderr, "%s\n", "play_basic_17S_DAS: invalid arguments\n");
        return NULL;
    }

    deck=table_getDeck(table);
    crupier=table_getCrupier(table);
    playerHand=player_getHand(player, 0);
    crupierHand=crupier_getHand(crupier);

    crupierHand_values=hand_getValues(crupierHand);
    crupierCard=crupierHand_values[0];
    if (crupierCard <= 0) return NULL;
    free (crupierHand_values);

    while (currentHand < numHands){
        /* error handling */
        if (!playerHand) return NULL;
        /* refresh of flags and values */
        split_current=false;
        stop=false;
        split_possible=hand_splitIsPossible(playerHand);

        if (playerHand_values) free (playerHand_values);
        playerHand_values=hand_getValues(playerHand);

        /* BlackJack check and payment */
        if (hand_getNumCards(playerHand)==2 && playerHand_values[0]==21 && split_once==false){
            player_addCash(player, 2.5*hand_getCurrentBet(playerHand));
            stop=true;
        }
        while (stop == false){
            /* refresh of flags and values */
            if (playerHand_values) free (playerHand_values);
            playerHand_values=hand_getValues(playerHand);
            hard_hand=hand_isHard(playerHand);
            split_possible=hand_splitIsPossible(playerHand);

            /* split_possibility */
            if (split_possible == true && (numHands < MAX_HANDS)){
                playerPoints = hard_hand? playerHand_values[0]:playerHand_values[1];
                switch (playerPoints) {
                    case 2:
                        split_current=strategy_split(player, currentHand, deck);
                        split_once = true;
                        numHands++;
                        break;
                    case 4:
                    case 6:
                        if (crupierCard <= 7){
                            split_current=strategy_split(player, currentHand, deck);
                            split_once=true;
                            numHands++;
                        }else if (crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n",  "play_basic_17S_DAS: error in switch split, case 2-2 or 3-3");
                            return NULL;
                        }
                        break;
                    case 8:
                        if (crupierCard == 5 || crupierCard == 6){
                            split_current=strategy_split(player, currentHand, deck);
                            split_once=true;
                            numHands++;
                        }else if (crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n",  "play_basic_17S_DAS: error in switch split, case 4-4");
                            return NULL;
                        }
                        break;
                    case 10:
                        if (crupierCard <= 9){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if (crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n",  "play_basic_17S_DAS: error in switch split, case 5-5");
                            return NULL;
                        }
                        break;
                    case 12:
                    case 14:
                        if (crupierCard <= 7){
                            split_current=strategy_split(player, currentHand, deck);
                            split_once=true;
                            numHands++;
                        }else if (crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n",  "play_basic_17S_DAS: error in switch split, case 6-6 or 7-7");
                            return NULL;
                        }
                        break;
                    case 16:
                        split_current=strategy_split(player, currentHand, deck);
                        split_once = true;
                        numHands++;
                        break;
                    case 18:
                        if (crupierCard == 7 || crupierCard == 10 || crupierCard == 11){
                            stop = true;
                        }else if (crupierCard <= 11){
                            split_current=strategy_split(player, currentHand, deck);
                            split_once=true;
                            numHands++;
                        }else{
                            fprintf(stderr, "%s\n",  "play_basic_17S_DAS: error in switch split, case 9-9");
                            return NULL;
                        }
                        break;
                    case 20:
                        stop = true;
                        break;
                    default:
                        fprintf(stderr, "%s [%d]\n", "play_basic_17S_DAS: error in switch split, default case reached with value:", playerPoints);
                        return NULL;
                }
            }else if (hard_hand == false){
                playerPoints = playerHand_values[0];
                switch (playerPoints) {
                    case 13: /* A-2 (11+3) */
                    case 14: /* A-3 (11+3) */
                        if(crupierCard == 5 || crupierCard == 6){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch soft, case A-2, A-3");
                            return NULL;
                        }
                        break;
                    case 15:
                    case 16:
                        if(crupierCard == 4 || crupierCard == 5 || crupierCard == 6){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch soft, case A-4, A-5");
                            return NULL;
                        }
                        break;
                    case 17:
                        if(crupierCard == 3 ||crupierCard == 4 || crupierCard == 5 || crupierCard == 6){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch soft, case A-6");
                            return NULL;
                        }
                        break;
                    case 18:
                        if(crupierCard == 3 ||crupierCard == 4 || crupierCard == 5 || crupierCard == 6){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if(crupierCard == 2 || crupierCard == 7 || crupierCard == 8){
                            stop = true;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch soft, case A-7");
                            return NULL;
                        }
                        break;
                    case 19:
                    case 20:
                    case 21:
                        stop = true;
                        break;
                    default:
                        fprintf(stderr, "%s [%d]\n", "play_basic_17S_DAS: error in switch soft, reached with value:", playerPoints);
                        return NULL;
                }
            }else if (hard_hand == true){
                playerPoints = playerHand_values[0] <= 21? playerHand_values[0]:playerHand_values[1];

                switch (playerPoints) {
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                        code=strategy_hit(player, currentHand, playerHand, deck);
                        if (code == FAIL) return NULL;
                        break;
                    case 9:
                        if(crupierCard == 3 ||crupierCard == 4 || crupierCard == 5 || crupierCard == 6){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch hard, case 9");
                            return NULL;
                        }
                        break;
                    case 10:
                        if(crupierCard == 10 ||crupierCard == 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else if(crupierCard <= 11){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch hard, case 10");
                            return NULL;
                        }
                        break;
                    case 11:
                        if(crupierCard == 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else if(crupierCard < 11){
                            stop=strategy_double(player, currentHand, playerHand, deck);
                            if ( stop == false ) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch hard, case 11");
                            return NULL;
                        }
                        break;
                    case 12:
                        if(crupierCard == 4 ||crupierCard == 5 || crupierCard == 6){
                            stop = true;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch hard, case 12");
                            return NULL;
                        }
                        break;
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                        if(crupierCard <= 6){
                            stop = true;
                        }else if(crupierCard <= 11){
                            code=strategy_hit(player, currentHand, playerHand, deck);
                            if (code == FAIL) return NULL;
                        }else{
                            fprintf(stderr, "%s\n", "play_basic_17S_DAS: error in switch hard, case 13, 14, 15, 16");
                            return NULL;
                        }
                        break;
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                        stop=true;
                        break;
                    default:
                        fprintf(stderr, "%s [%d]\n", "play_basic_17S_DAS: error in switch hard, default case reached with value:", playerPoints);
                        return NULL;
                }
            }else{
                fprintf(stderr, "%s\n", "play_basic_17S_DAS: error, hand is neither hard or soft");
                return NULL;
            }

            if (playerHand_values) free (playerHand_values);
            playerHand_values=hand_getValues(playerHand);

            if (playerHand_values[0]>21 && playerHand_values[1]==-1){
                stop = true;
            }
            if (playerHand_values[0]>21 && playerHand_values[1]>21){
                stop = true;
            }
        }
        currentHand++;
        playerHand=player_getHand(player, currentHand);
    }
    return player;
}
