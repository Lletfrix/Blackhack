/* main.c*/
#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

// load strategies
#include "bet_strategies.h"
#include "play_strategies.h"

#define NUMPLAYERS 4

void usage() {
    printf("usage: ./main [games] [seed]\n");
}

int main(int argc, char** argv) {
    int numPartidas;

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
    table = table_addPlayer(table, player_ini(double_if_lose, play_random));
    if(!table){
        fprintf(stderr, "main: ERROR in table_addPlayer.\n");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < NUMPLAYERS; i++) {
        // pasar funciones vacías para que de momento compile
        Player *p = player_ini(double_if_lose, play_like_crupier);
        if (!p) {
            fprintf(stderr, "main: player_ini: ERROR allocating memory for player %d\n", i);
            table_destroy(table);
            return EXIT_FAILURE;
        }

        table = table_addPlayer(table, p);
        if(!table){
            fprintf(stderr, "main: ERROR in table_addPlayer. Iteration: %d\n", i);
            return EXIT_FAILURE;
        }
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
        table = table_makeBets(table);
        if(!table){
            fprintf(stderr, "Error in makeBets.\n");
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
        //table_printLastGame(stdout, table, k);
    }


table_printPlayersPercentages(stdout, table);
fprintf(stdout, "\n");
table_printPlayersMoney(stdout, table);
fprintf(stdout, "\n");
table_printPlayersStreakData(stdout, table);

table_destroy(table);
return EXIT_SUCCESS;
}
