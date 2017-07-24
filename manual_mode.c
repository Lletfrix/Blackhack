#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"

int main(){
  int nplayers;
  Table* table=NULL;

  table=table_

  fprintf(stdout, "¿Cuantos jugadores?\n");
  scanf("%d", &nplayers);

  table = table_addPlayer(table, player_ini(manual_bet, play_manual));
  if(!table){
      fprintf(stderr, "main: ERROR in table_addPlayer.\n");
      return EXIT_FAILURE;
  }

  for(int i=1;i<nplayers;i++){
    table = table_addPlayer(table, player_ini(never_bets, play_manual));
    if(!table){
        fprintf(stderr, "main: ERROR in table_addPlayer.\n");
        return EXIT_FAILURE;
    }
  }

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


  while(1) {

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

  table_printPlayersPercentages(stdout, table);
  fprintf(stdout, "\n");
  table_printPlayersMoney(stdout, table);
  fprintf(stdout, "\n");
  table_printPlayersStreakData(stdout, table);

  table_destroy(table);
  return EXIT_SUCCESS;
}
