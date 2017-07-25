#include "../play_strategies.h"

Player *play_manual(Player *p, Table *t){
  double *listofprob;
  Deck *deck=NULL;
  Crupier *crupier=NULL;
  Hand *playerHand=NULL, *crupierHand=NULL;
  int *playerHand_values=NULL, *crupierHand_values=NULL;

  deck=table_getDeck(t);
  crupier=table_getCrupier(t);
  playerHand=player_getHand(p, 0);
  crupierHand=crupier_getHand(crupier);

  playerHand_values=hand_getValues(playerHand);
  crupierHand_values=hand_getValues(crupierHand);

  listofprob=probability_listofcards(deck);
  printf("Tu mano actual:\t%d %d\n", playerHand_values[0], playerHand_values[1]);
  printf("Mano del crupier:%d %d\n", crupierHand_values[0], crupierHand_values[1]);

  /*
  printf("Lista de probabilidad:\n");
  printf("\tAS\t%lf\t%lf%%\n", listofprob[0], listofprob[0]*100);
  printf("\t2\t%lf\t%lf%%\n", listofprob[1],  listofprob[1]*100);
  printf("\t3\t%lf\t%lf%%\n", listofprob[2],  listofprob[2]*100);
  printf("\t4\t%lf\t%lf%%\n", listofprob[3],  listofprob[3]*100);
  printf("\t5\t%lf\t%lf%%\n", listofprob[4],  listofprob[4]*100);
  printf("\t6\t%lf\t%lf%%\n", listofprob[5],  listofprob[5]*100);
  printf("\t7\t%lf\t%lf%%\n", listofprob[6],  listofprob[6]*100);
  printf("\t8\t%lf\t%lf%%\n", listofprob[7],  listofprob[7]*100);
  printf("\t9\t%lf\t%lf%%\n", listofprob[8],  listofprob[8]*100);
  printf("\tT\t%lf\t%lf%%\n", listofprob[9],  listofprob[9]*100);
  */

  printf("\tAS\t%lf\t%lf%%\n", listofprob[0], listofprob[0]*100);
  for(int i=1;i<9;i++){
    printf("\t%d\t%lf\t%lf%%\n", i+1,listofprob[i],  listofprob[i]*100);
  }
  printf("\tT\t%lf\t%lf%%\n", listofprob[9],  listofprob[9]*100);

  return p;
}
