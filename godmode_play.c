
#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

// load strategies
#include "bet_strategies.h"
#include "play_strategies.h"

void usage(FILE* pf){
    if(!pf){
        return;
    }
    fprintf(pf, "usage: ./main [games]\n");
}

void print_currentStatus(FILE *pf, FILE* f_in, Deck *deck, Player *player, Crupier *crupier){
    char option;
    fprintf(pf, "Would you like to print the current status?\n\t[y]yes\n\t[n]no\nMake your decision: ");
    fscanf(f_in,"\n%c",&option);
    switch (option) {
        case 'y':
            break;
        case 'n':
            return;
        default:
            fprintf(pf, "You failed, choosing no as default.\n");
            return;
    }
    fprintf(pf, "\n---------------------------\n");
    fprintf(pf, "Current status:\n\n");
    fprintf(pf, "\t Deck:\n\t");
    deck_print(pf,deck);
    fprintf(pf, "\n");
    fprintf(pf, "\t Player:\n\t");
    player_print(pf,player);
    fprintf(pf, "\n");
    fprintf(pf, "\t Crupier:\n\t");
    crupier_print(pf, crupier);
    fprintf(pf, "\n");
    fprintf(pf, "\n---------------------------\n");
}

void test_functions(FILE *pf, FILE *f_in, Deck *deck, Player *player, Crupier *crupier){
    char option;
    fprintf(pf, "Would you like to test functions?\n\t[y]yes\n\t[n]no\nMake your decision: ");
    fscanf(f_in,"\n%c",&option);
    switch (option) {
        case 'y':
            break;
        case 'n':
            return;
        default:
            fprintf(pf, "You failed, choosing no as default.\n");
            return;
    }
    while(1){
        fprintf(pf,"Options:\n\t[d]test deck\n\t[p]test player\n\t[h]test hand\n\t[c]test crupier\n\t[n]nothing\nMake yours: ");
        fscanf(f_in,"\n%c",&option);
        switch (option) {
            case 'd':
                /* deck functions */
                break;
            case 'p':
                /* player functions */
                break;
            case 'h':
                /* hand functions */
                break;
            case 'c':
                /* crupier functions */
                break;
            case 'n':
                /*do nothing*/
                return;
            default:
            ;
        }
    }
}

int main (int argc, char** argv){
    FILE *pf=stdout, *f_in=stdin;
    Deck *deck;
    Player *p1, *pErr;
    char option;
    Crupier *c;
    //Hand *hAux;
    Table *table;
    int *hv, rank, numPartidas, handNumber;
    bool split=false, stop=false;
    //Peg *condition;
    long bet, betOrg;//, beth0, beth1;
    if (argc<2){
        fprintf(pf, "Playing 10 games as default");
        usage(pf);
        numPartidas=10;
    }
    else{
        numPartidas=atoi(argv[1]);
    }

    //Initializations

    deck=deck_ini(time(NULL));
    p1=player_ini(&never_bets, &play_do_nothing);
    c=crupier_ini();

    for (size_t i = 0; i < numPartidas; i++) {
        fprintf(pf, "\n*** STARTING NEW GAME ***\n\n");
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        fprintf(pf, "\n***DEALING FIRST CARD TO PLAYER***\n");
        player_addCardToHand(p1, 0, deck_draw(deck));
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        fprintf(pf, "\n***DEALING SECOND CARD TO PLAYER***\n");
        player_addCardToHand(p1, 0, deck_draw(deck));
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        fprintf(pf, "\n***DEALING FIRST CARD TO CRUPIER***\n");
        crupier_addCard(c, deck_draw(deck));
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        fprintf(pf, "\n*** MAKE YOUR BET***\n");
        fprintf(pf, "Input: ");
        fscanf(f_in, "%ld", &bet);
        betOrg=bet;
        player_removeCash(p1, bet);
        player_setLastBet(p1, bet);
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        /*********************************************************************/
        fprintf(pf, "\n***TIME TO PLAY***\n");

        fprintf(pf, "\n***HAND NUMBER 0***\n");
        handNumber=0;
        hv=hand_getValues(player_getHand(p1, handNumber));
        while(hv[1]<=21){
            fprintf(pf, "Player hands:\n");
            hand_print(pf,player_getHand(p1,handNumber));
            fprintf(pf, "Crupier hand:\n");
            hand_print(pf,crupier_getHand(c));

            fprintf(pf, "\nYour turn, what would you like to do?\n\t[h]hit\n\t[s]split\n\t[d]double\n\t[t]stop\nInput: ");
            fscanf(f_in,"\n%c",&option);
            switch (option) {
                case 'h':
                    rank=deck_draw(deck);
                    fprintf(pf, "Card drawed is: [%d]\n", rank);
                    player_addCardToHand(p1, handNumber, rank);
                    break;
                case 's':
                    pErr=player_splitHand(p1, handNumber);
                    if(!pErr){
                        fprintf(stderr, "Splitting was impossible\n");
                        break;
                    }
                    player_removeCash(p1, bet);
                    split=true;
                    break;
                case 'd':
                    fprintf(pf, "Doubling bet in first hand\n");
                    player_removeCash(p1, 2*bet);
                    player_setLastBet(p1, 3*bet);
                    bet*=3;
                    break;
                case 't':
                    fprintf(pf, "Ending this hand play\n");
                    stop=true;
                    break;
                default:
                ;
            }
            if(stop==true){
                break;
            }
            free(hv);
            hv=hand_getValues(player_getHand(p1, handNumber));
            //beth0=bet;
            print_currentStatus(pf, f_in, deck, p1, c);
            test_functions(pf, f_in, deck, p1, c);
        }
        if(split==true){
            fprintf(pf, "\n***HAND NUMBER 1***\n");
            handNumber=1;
            bet=betOrg;
            free(hv);
            hv=hand_getValues(player_getHand(p1, handNumber));
            while(hv[1]<=21){
                fprintf(pf, "Player hand:\n");
                hand_print(pf, player_getHand(p1,handNumber));
                fprintf(pf, "Crupier hand:\n");
                hand_print(pf, crupier_getHand(c));

                fprintf(pf, "\nYour turn, what would you like to do?\n\t[h]hit\n\t[d]double\n\t[t]stop\nInput: ");
                fscanf(f_in,"\n%c",&option);
                switch (option) {
                    case 'h':
                        rank=deck_draw(deck);
                        fprintf(pf, "Card drawed is: [%d]\n", rank);
                        player_addCardToHand(p1, handNumber, rank);
                        break;
                    case 'd':
                        fprintf(pf, "Doubling bet in first hand\n");
                        player_removeCash(p1, 2*bet);
                        player_setLastBet(p1, 3*bet);
                        bet*=3;
                        break;
                    case 't':
                        fprintf(pf, "Ending this hand play\n");
                        stop=true;
                        break;
                    default:
                    ;
                }
                if(stop==true){
                    break;
                }
                free(hv);
                hv=hand_getValues(player_getHand(p1, handNumber));
                //beth1=bet;
                print_currentStatus(pf, f_in, deck, p1, c);
                test_functions(pf, f_in, deck, p1, c);
            }
        }
        free (hv);
        /*********************************************************************/
        /*fprintf(pf, "\n***DEALING SECOND CARD TO CRUPIER\n");
        crupier_addCard(c, deck_draw(deck));
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);*/
        fprintf(pf, "\n***CRUPIER PLAYS***\n");
        crupier_play(c, deck);
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
        fprintf(pf, "\n***DISTRIBUTE EARNINGS***\n");
        /*for (size_t i = 0; i < player_getNHands(p1); i++) {
            hAux=player_getHand(p1, i);
            hv=hand_getValues(player_getHand(p1, i));
            if (hv[0]==21 && hand_getNumCards(hAux)==2){
                if(i==0){
                    fprintf(pf, "\nBlackJack in hand 0!\n");
                    player_addCash(p1, 2.5*beth0);
                    print_currentStatus(pf, f_in, deck, p1, c);
                    test_functions(pf, f_in, deck, p1, c);
                    continue;
                }
                fprintf(pf, "\nBlackJack in hand 1!\n");
                player_addCash(p1, 2,5*beth1);
                print_currentStatus(pf, f_in, deck, p1, c);
                test_functions(pf, f_in, deck, p1, c);
                continue;
            }
        }
        condition=player_handsCondition(c, p1);
        for (size_t i = 0; i < player_getNHands(p1); i++) {
            if (condition[i]==WIN) {
                if(i==0){
                    player_addCash(p1, 2*beth0);
                    contiue;
                }
                player_addCash(p1, 2*beth1);
                contiue;
            }
            if (condition[i]==TIE) {
                if(i==0){
                    player_addCash(p1, beth0);
                    continue;
                }
                player_addCash(p1, beth1);
                continue;
            }
        }
        free(condition);
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);*/
        table=table_ini();
        table_addPlayer(table, p1);
        table_setCrupier(table, c);
        table_setDeck(table, deck);
        table_distributeEarnings(table);
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);

        fprintf(pf, "\n*** RESTARTING GAME***\n");
        table_restartTable(table);
        print_currentStatus(pf, f_in, deck, p1, c);
        test_functions(pf, f_in, deck, p1, c);
    }
    deck_destroy(deck);
    player_destroy(p1);
    crupier_destroy(c);
    table_destroy(table);
    return EXIT_SUCCESS;
}
