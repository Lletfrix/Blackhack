#include "../play_strategies.h"

Player *play_manual(Player *p, Table *t){
  double *listofprob;
  Deck *deck=NULL;
  Crupier *crupier=NULL;
  Hand *playerHand=NULL, *crupierHand=NULL;
  int *playerHand_values=NULL, *crupierHand_values=NULL, rank;
  char option;

  deck=table_getDeck(t);
  crupier=table_getCrupier(t);
  playerHand=player_getHand(p, 0);
  crupierHand=crupier_getHand(crupier);

  playerHand_values=hand_getValues(playerHand);
  crupierHand_values=hand_getValues(crupierHand);

  listofprob=probability_listofcards(deck);
  printf("Tu mano actual:\t%d %d\n", playerHand_values[0], playerHand_values[1]);
  printf("Mano del crupier:%d %d\n", crupierHand_values[0], crupierHand_values[1]);

  printf("\tAS\t%lf\t%lf%%\n", listofprob[0], listofprob[0]*100);
  for(int i=1;i<9;i++){
    printf("\t%d\t%lf\t%lf%%\n", i+1,listofprob[i],  listofprob[i]*100);
  }
  printf("\tT\t%lf\t%lf%%\n", listofprob[9],  listofprob[9]*100);

  if(playerHand_values[0]==21){
      player_addCash(p, 2.5*hand_getCurrentBet(playerHand));
      return p;
  }
  printf("La estrategia recomienda: %c\n", number_strategy[PLAYER_CONVERT(playerHand_values[0])][CRUPIER_CONVERT(crupierHand_values[0])]);

  printf("¿Que quieres hacer?:\n");
  printf("\t(H)it\n\t(S)tand\n\t(D)ouble\n");
  scanf("\n%c", &option);

  if(option=='s' || option=='S'){
    return p;
  }
  else if(option=='d' || option=='D'){
    fprintf(stdout, "¿Que carta(rank) te ha salido?\n");
    fscanf(stdin, "%d", &rank);
    hand_insertCard(playerHand, rank);
    deck_removeCard(deck, rank);
    return p;
  }
  else{
    while(option=='h'||option=='H'){
      fprintf(stdout, "¿Que carta(rank) te ha salido?\n");
      fscanf(stdin, "%d", &rank);
      hand_insertCard(playerHand, rank);
      deck_removeCard(deck, rank);

      playerHand_values=hand_getValues(playerHand);

      listofprob=probability_listofcards(deck);
      printf("Tu mano actual:\t%d %d\n", playerHand_values[0], playerHand_values[1]);
      printf("Mano del crupier:%d %d\n", crupierHand_values[0], crupierHand_values[1]);

      printf("\tAS\t%lf\t%lf%%\n", listofprob[0], listofprob[0]*100);
      for(int i=1;i<9;i++){
        printf("\t%d\t%lf\t%lf%%\n", i+1,listofprob[i],  listofprob[i]*100);
      }
      printf("\tT\t%lf\t%lf%%\n", listofprob[9],  listofprob[9]*100);

      printf("¿Que quieres hacer?:\n");
      printf("\t(H)it\n\t(S)tand\n");
      scanf("\n%c", &option);
    }
    return p;
  }

  return p;
}
