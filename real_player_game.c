/* main.c*/
#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

// load strategies
#include "bet_strategies.h"
#include "play_strategies.h"

#define NUMPLAYERS 1

void usage() {
    printf("usage: ./main [games] [seed]\n");
}

int main(int argc, char** argv) {
    int numPartidas;
    int *handValues;
    long int bet;
    char option;
    //Table* tableError;

    if (argc > 3) {
        usage();
        return -1;
    }
    if (argc < 2){
    fprintf(stderr, "Playing a hundred games as default.\n");
    numPartidas=100;
    }
    else numPartidas=atoi(argv[1]);
    Table *table = table_ini();

    if(!table)
    {
        fprintf(stderr, "main: error: failed to initialize table\n");
        return EXIT_FAILURE;
    }

    // initialize players
    // set the first player to use a random strategy
    table = table_addPlayer(table, player_ini(boring_bet, play_basic_17S_DAS_matrix));
    if(!table){
        fprintf(stderr, "main: ERROR in table_addPlayer.\n");
        return EXIT_FAILURE;
    }

    // initialize crupier
    Crupier *crupier = crupier_ini();
    if (!crupier) {
        fprintf(stderr, "main: crupier_ini: error allocating memory for crupier\n");
        table_destroy(table);
        return 3;
    }
    table = table_setCrupier(table, crupier);
    if(!table){
        fprintf(stderr, "main: ERROR setting Crupier.\n");
        return EXIT_FAILURE;
    }
    // initialize deck, seeding with seed if provided
    unsigned seed;
    if (argc == 3) {
        seed = (unsigned) atoi(argv[2]);
    } else {
        seed = time(NULL);
    }
    printf("seeded game with %u\n", seed);

    Deck *deck = deck_ini(seed);
    if (!deck) {
        fprintf(stderr, "main: deck_ini: error allocating memory of deck\n");
        table_destroy(table);
        return 4;
    }
    table = table_setDeck(table, deck);
    if(!table){
        fprintf(stderr, "Error setting Deck.\n");
        return EXIT_FAILURE;
    }
    // play a hundred times
    for (int k = 0; k < numPartidas; k++) {

        /* apostar*/
        /*tableError = table_makeBets(table);
        if(!tableError){
            fprintf(stderr, "Error in makeBets.\n");
            table_printLastGame(stderr, table, k);
            return EXIT_FAILURE;
        }*/

        fprintf(stdout, "\n *NEW GAME*\n");
        fprintf(stdout, "Actual cash: %ld\n", player_getCash(table_getPlayer(table, 0)));
        fprintf(stdout, "Make your bet:\n");
        fscanf(stdin, "%ld", &bet);
        player_removeCash(table_getPlayer(table, 0), bet);
        player_setLastBet(table_getPlayer(table, 0), bet);

        /* repartir primera carta */
        table = table_dealCard(table);
        if(!table){
            fprintf(stderr, "Error in dealCard.\n");
            return EXIT_FAILURE;
        }
        /* repartir 1 carta al crupier */
        table = table_dealCardToCrupier(table);
        if(!table){
            fprintf(stderr, "Error in dealCardToCrupier.\n");
            return EXIT_FAILURE;
        }
        /*repartir segunda carta*/
        table = table_dealCard(table);
        if(!table){
            fprintf(stderr, "Error in dealCard2.\n");
            return EXIT_FAILURE;
        }
        /* juegan el jugador */

        handValues=hand_getValues(player_getHand(table_getPlayer(table,0),0));
        fprintf(stdout, "\nHand Value(0): %d\n", handValues[0]);
        fprintf(stdout, "Hand Value(1): %d\n", handValues[1]);
        fprintf(stdout, "What do you want?\n");
        fprintf(stdout, "Hit('h').\nStop('s')\n");
        fscanf(stdin, "\n%c", &option);
        while(option=='h'&& handValues[0]<21){
          player_addCardToHand(table_getPlayer(table,0),0, deck_draw(deck));
          free(handValues);
          handValues =hand_getValues(player_getHand(table_getPlayer(table,0),0));
          if (!handValues) {
              fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
              free(handValues);
              return EXIT_FAILURE;
          }
          fprintf(stdout, "\nHand Value(0): %d\n", handValues[0]);
          fprintf(stdout, "Hand Value(1): %d\n", handValues[1]);
          if(handValues[0]>21){
            fprintf(stdout, "YOU WENT TO FAR LOL LOOSER\n");
            break;
          }
          fprintf(stdout, "What do you want?\n");
          fprintf(stdout, "Hit('h').\nStop('s')\n");
          fscanf(stdin, "\n%c", &option);
        }
        if(handValues[1]>0){
          fprintf(stdout, "What do you want?\n");
          fprintf(stdout, "Hit('h').\nStop('s')\n");
          fscanf(stdin, "\n%c", &option);
          while(option=='h'&& handValues[1]<21){
            player_addCardToHand(table_getPlayer(table,0),0, deck_draw(deck));
            free(handValues);
            handValues =hand_getValues(player_getHand(table_getPlayer(table,0),0));
            if (!handValues) {
                fprintf(stderr, "play_standard_wo_sd: handValues pointing NULL\n");
                free(handValues);
                return EXIT_FAILURE;
            }
            fprintf(stdout, "\nHand Value(0): %d\n", handValues[0]);
            fprintf(stdout, "Hand Value(1): %d\n", handValues[1]);
            if(handValues[1]>21){
              fprintf(stdout, "YOU WENT TO FAR LOL LOOSER\n");
              break;
            }
            fprintf(stdout, "What do you want?\n");
            fprintf(stdout, "Hit('h').\nStop('s')\n");
            fscanf(stdin, "\n%c", &option);
          }
        }
        free(handValues);

        /*juega el crupier*/
        crupier = crupier_play(crupier, deck);
        handValues=hand_getValues(crupier_getHand(crupier));
        fprintf(stdout, "\nCrupier's Hand:\n");
        fprintf(stdout, "Hand Value(0): %d\n", handValues[0]);
        fprintf(stdout, "Hand Value(1): %d\n", handValues[1]);
        free(handValues);

        table = table_distributeEarnings(table);
        if(!table){
            fprintf(stderr, "Error in distributeEarnings.\n");
            return EXIT_FAILURE;
        }
        table = table_restartTable(table);
        if(!table){
            fprintf(stderr, "Error in restartTable.\n");
            return EXIT_FAILURE;
        }
        fprintf(stdout, "\n");
        table_printLastGame(stdout, table, k);
    }


table_printPlayersPercentages(stdout, table);
fprintf(stdout, "\n");
table_printPlayersMoney(stdout, table);
fprintf(stdout, "\n");
table_printPlayersStreakData(stdout, table);

table_destroy(table);
return EXIT_SUCCESS;
}
