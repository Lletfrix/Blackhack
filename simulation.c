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
    printf("usage: ./main [games] [filetowrite] [seed]\n");
}

int main(int argc, char** argv) {
    int numPartidas;
    Table* tableError;
    FILE* output;

    if (argc > 4) {
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
    table = table_addPlayer(table, player_ini(boring_bet, play_basic_17S_DAS_matrix));
    if(!table){
        fprintf(stderr, "main: ERROR in table_addPlayer.\n");
        return EXIT_FAILURE;
    }
    table = table_addPlayer(table, player_ini(boring_bet, play_like_crupier));
    if(!table){
        fprintf(stderr, "main: ERROR in table_addPlayer.\n");
        return EXIT_FAILURE;
    }
    table = table_addPlayer(table, player_ini(boring_bet, play_like_crupier));
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
    if (argc == 4) {
        seed = (unsigned) atoi(argv[3]);
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
    if (argc > 2){
        output = fopen(argv[2], "w");
        if (!output) return 0;
    }

    // play a hundred times
    for (int k = 0; k < numPartidas; k++) {

        /* apostar*/
        tableError = table_makeBets(table);
        if(!tableError){
            fprintf(stderr, "Error in makeBets.\n");
            table_printLastGame(stderr, table, k);
            return EXIT_FAILURE;
        }

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
        /* juegan los jugadores */
        table = table_makePlays(table);
        if(!table){
            fprintf(stderr, "Error in makePlays.\n");
            return EXIT_FAILURE;
        }
        /*juega el crupier*/ // TODO: should we only touch the crupier thorugh table?
        crupier = crupier_play(crupier, deck);
        if(!crupier){
            fprintf(stderr, "Error while crupier is playing.\n");
            return EXIT_FAILURE;
        }
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
        /*table_printLastGame(stdout, table, k);*/
        if (argc > 2){
            table_printCash(output, table);
        }
    }

if (argc > 2){
    fclose(output);
}

table_printPlayersPercentages(stdout, table);
fprintf(stdout, "\n");
table_printPlayersMoney(stdout, table);
fprintf(stdout, "\n");
table_printPlayersStreakData(stdout, table);

table_destroy(table);
return EXIT_SUCCESS;
}
