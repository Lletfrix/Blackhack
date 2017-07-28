#include "player.h"
#include "crupier.h"
#include "hand.h"
#include "deck.h"


// load strategies
#include "bet_strategies.h"
#include "play_strategies.h"

int main(int argc, char **argv){
  //FILE *output = stdout;
  int nplayers=1, rank;/*handValues*/
  Table* table=NULL, *tableError;
  Hand *playerHand=NULL, *crupierHand=NULL;
  /*
  fprintf(stdout, "¿Cuantos jugadores?\n");
  scanf("%d", &nplayers);
  */
  table=table_ini();

  table = table_addPlayer(table, player_ini(never_bets, play_manual));
  if(!table){
      fprintf(stderr, "main: ERROR in table_addPlayer 1.\n");
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

  Deck *deck = deck_ini(time(NULL));
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


  for (int k = 0; k >-1; k++) {
      playerHand=player_getHand(table_getPlayer(table, 0), 0);
      crupierHand=crupier_getHand(crupier);

      /* apostar*/
      tableError = table_makeBets(table);
      if(!tableError){
          fprintf(stderr, "Error in makeBets.\n");
          table_printLastGame(stderr, table, k);
          return EXIT_FAILURE;
      }

      /* repartir primera carta */
      fprintf(stdout, "¿Que PRIMERA carta(rank) tienes?\n");
      fscanf(stdin, "%d", &rank);
      hand_insertCard(playerHand, rank);
      deck_removeCard(deck, rank);

      fprintf(stdout, "¿Que SEGUNDA carta(rank) tienes?\n");
      fscanf(stdin, "%d", &rank);
      hand_insertCard(playerHand, rank);
      deck_removeCard(deck, rank);

      fprintf(stdout, "¿Que carta(rank) tiene el CRUPIER?\n");
      fscanf(stdin, "%d", &rank);
      hand_insertCard(crupierHand, rank);
      deck_removeCard(deck, rank);

      /* juegan los jugadores */
      table = table_makePlays(table);
      if(!table){
          fprintf(stderr, "Error in makePlays.\n");
          return EXIT_FAILURE;
      }
      /*juega el crupier*/ // TODO: should we only touch the crupier thorugh table?

      /*
      printf("¿Cual es la segunda carta del crupier?\n");
      fscanf(stdin, "%d", &rank);
      hand_insertCard(crupierHand, rank);
      deck_removeCard(deck, rank);
      handValues=hand_getValues(crupierHand);
      printf("Mano del crupier:%d %d\n", handValues[0], handValues[1]);

      while (handValues[0] < 17) {
        printf("Que carta le ha salido al crupier?\n");
        fscanf(stdin, "%d", &rank);
        hand_insertCard(crupierHand, rank);
        deck_removeCard(deck, rank);
        handValues=hand_getValues(crupierHand);
        printf("Mano del crupier:%d %d\n", handValues[0], handValues[1]);
      }
      */
      /*Si no sobrepasa 21 devuelve c, si se pasa, comprueba que existe el segundo
       valor, (As=1) y pide cartas igual que antes*/

      /*
      if (handValues[0] > 21) {
          if (handValues[1] > 0) {
              while (handValues[1] < 17) {
                  free(handValues);
                  printf("Que carta le ha salido al crupier?\n");
                  fscanf(stdin, "%d", &rank);
                  hand_insertCard(crupierHand, rank);
                  deck_removeCard(deck, rank);
                  handValues=hand_getValues(crupierHand);
                  printf("Mano del crupier:%d %d\n", handValues[0], handValues[1]);
              }
              free(handValues);
          }
      }


      table = table_distributeEarnings(table);
      if(!table){
          fprintf(stderr, "Error in distributeEarnings.\n");
          return EXIT_FAILURE;
      }
      */
      table = table_restartTable(table);
      if(!table){
          fprintf(stderr, "Error in restartTable.\n");
          return EXIT_FAILURE;
      }
      /*
      table_printLastGame(stdout, table, k);
      if (argc > 2){
          table_printCash(output, table);
      }
      */
  }

  table_printPlayersPercentages(stdout, table);
  fprintf(stdout, "\n");
  table_printPlayersMoney(stdout, table);
  fprintf(stdout, "\n");
  table_printPlayersStreakData(stdout, table);

  table_destroy(table);
  return EXIT_SUCCESS;
}
