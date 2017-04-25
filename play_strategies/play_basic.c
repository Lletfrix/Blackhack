#include "../play_strategies.h"

bool player_double(Player* p, int currentHand, Deck* deck){
    if(NULL==player_removeCash(p, player_getLastBet(p))){
        return false;
    }
    if(NULL==player_addCardToHand(p, currentHand, deck_draw(deck))){
        return false;
    }
    return true;
}

Status player_hit(Player* p, int currentHand, Deck* deck){
    if(NULL==player_addCardToHand(p, currentHand, deck_draw(deck))){
        return FAIL;
    }
    return OK;
}

bool player_split(Player* p,int currentHand, Deck* deck){
    if(NULL==player_splitHand(p, currentHand)){
        return false;
    }
    if(NULL==player_addCardToHand(p, currentHand, deck_draw(deck))){
        return false;
    }
    return true;
}


Player *play_basic(Player* p, Table* t){
    Deck *deck;
    Hand *playerHand, *crupierHand;
    int *pHandValues, *cHandValues, cCard, numHands=1, currentHand=0;
    bool y_n_doubled=false, y_n_stand=false, y_n_split=false, y_n_hard=true;
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
    crupierHand=crupier_getHand(table_getCrupier(t));

    pHandValues=hand_getValues(playerHand);
    cHandValues=hand_getValues(crupierHand);
    cCard=cHandValues[0];
    free(cHandValues);

    while(currentHand<numHands){

        /* Make y_n_hard false if there is a soft hand */
        if((pHandValues[1]!=-1)&&(pHandValues[0]<=21)){
            y_n_hard=false;
        }else{
            y_n_hard=true;
        }
        /* Split possibility */
        if((hand_splitIsPossible(player_getHand(p, currentHand))==true) && (numHands < MAX_HANDS)){
            switch (pHandValues[1]) {
                case 2: /* A-A */
                    y_n_split=player_split(p, currentHand, deck);
                    if(y_n_split==true){
                        numHands++;
                    }
                    break;

                case 4: /* 2-2 */
                case 6: /* 3-3 */
                    if(cCard==8||cCard==9||cCard==10||cCard==11){
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }else{
                        y_n_split=player_split(p, currentHand, deck);
                        if(y_n_split==true){
                            numHands++;
                        }
                    }
                    break;

                case 8: /* 4 - 4 */
                    if(cCard==5 || cCard == 6){
                        y_n_split=player_split(p, currentHand, deck);
                        if(y_n_split==true){
                            numHands++;
                        }
                    }else{
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }
                    break;

                case 10: /* 5-5 */
                    if(cCard==10||cCard==11){
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }else{
                        y_n_doubled=player_double(p, currentHand, deck);
                    }
                    break;

                case 12: /* 6-6 */
                case 14: /* 7-7 */
                    if(cCard==8||cCard==9||cCard==10||cCard==11){
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }else{
                        y_n_split=player_split(p, currentHand, deck);
                        if(y_n_split==true){
                            numHands++;
                        }
                    }
                    break;

                case 16: /* 8-8 */
                    y_n_split=player_split(p, currentHand, deck);
                    if(y_n_split==true){
                        numHands++;
                    }
                    break;

                case 18: /* 9-9 */
                    if(cCard==7||cCard==10||cCard==11){
                        y_n_stand=true;
                    }else{
                        y_n_split=player_split(p, currentHand, deck);
                        if(y_n_split==true){
                            numHands++;
                        }
                    }
                    break;

                case 20: /* 10-10 */
                    y_n_stand=true;
                    break;

                default:
                fprintf(stderr, "%s\n", "Split switch's default reached" );
                y_n_stand=true;
            }
            goto Next;
        }
        /* Soft Hand */
        if(y_n_hard==false){
            switch (pHandValues[1]) {
                case 3: /* A-2 */
                case 4: /* A-3 */
                    if(cCard==5||cCard==6){
                        y_n_doubled=player_double(p, currentHand, deck);
                    }else{
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }
                    break;

                case 5: /* A-4 */
                case 6: /* A-5 */
                    if(cCard==4||cCard==5||cCard==6){
                        y_n_doubled=player_double(p, currentHand, deck);
                    }else{
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }
                    break;

                case 7: /* A-6 */
                    if(cCard==3||cCard==4||cCard==5||cCard==6){
                        y_n_doubled=player_double(p, currentHand, deck);
                    }else{
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }
                    break;

                case 8: /* A-7 */
                    if(cCard==2||cCard==7||cCard==8){
                        y_n_stand=true;
                    }
                    else if(cCard==3||cCard==4||cCard==5||cCard==6){
                        y_n_doubled=player_double(p, currentHand, deck);
                    }else{
                        if(player_hit(p, currentHand, deck)==FAIL){
                            return NULL;
                        }
                    }
                    break;

                case 9: /* A-8 */
                case 10: /* A-9 */
                case 11: /* A-10 */
                    y_n_stand=true;
                    break;
                default:
                    fprintf(stderr, "%s\n", "Soft switch's default reached" );
                    y_n_stand=true;
            }
            goto Next;
        }

        /* Hard hand */
        switch (pHandValues[0]) {
            case 5:
            case 6:
            case 7:
            case 8:
                /* Hit */
                if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                break;

            case 9:
                if(cCard==3||cCard==4||cCard==5||cCard==6){
                    /* Double */
                    y_n_doubled=player_double(p, currentHand, deck);
                }else{
                    /* Hit */
                    if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                }break;

            case 10:
                if(cCard==10||cCard==11){
                    /* Hit */
                    if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                }else{
                    /* Double */
                    y_n_doubled=player_double(p, currentHand, deck);
                }break;

            case 11:
                if(cCard==11){
                    /* Hit */
                    if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                }else{
                    /* Double */
                    y_n_doubled=player_double(p, currentHand, deck);
                }break;

            case 12:
                if(cCard==4||cCard==5||cCard==6){
                    /* Stand */
                    y_n_stand=true;
                }else{
                    /* Hit */
                    if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                }break;

            case 13:
            case 14:
            case 15:
            case 16:
                if(cCard==2||cCard==3||cCard==4||cCard==5||cCard==6){
                    /* Stand */
                    y_n_stand=true;
                }else{
                    /* Hit */
                    if(player_hit(p, currentHand, deck)==FAIL) return NULL;
                }break;

            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                /* Stand */
                y_n_stand=true;
                break;

            default:
                fprintf(stderr, "%s\n", "Soft switch's default reached" );
                y_n_stand=true;
        }

        Next:
        /* refresh hand value */
        free(pHandValues);
        pHandValues=hand_getValues(playerHand);
        /* When you get over 21, you doubled or you decide to stand you are done with your hand */
        if (((pHandValues[0]>21) && (pHandValues[1]==-1)) || (pHandValues[1]>21) || (y_n_doubled==true) || (y_n_stand==true)) {
            currentHand++;
            y_n_doubled=false;
            y_n_stand=false;
        }
    }
    return p;
}
