#include "probability.h"

double probability_card(Deck *deck, int rank){
  double prob;
  int totalcards, rankcards;

  if(!deck){
    fprintf(stderr, "probability_card: invalid arguments pointing NULL\n");
    return -1;
  }

  totalcards=deck_getNCurrentCards(deck);
  rankcards=deck_getNCardsofRank(deck, rank);

  prob=(double)rankcards/totalcards;

  return prob;
}


double * probability_listofcards(Deck *deck){
  double *listprob=NULL;
  int i, totalcards, rankcards;

  if(!deck){
    fprintf(stderr, "probability_listofcards: invalid arguments pointing NULL\n");
    return NULL;
  }

  listprob=(double *)malloc(N_RANKS*sizeof(double));
  if(!listprob){
    fprintf(stderr, "probability_listofcards: ERROR allocating memory\n");
    return NULL;
  }

  totalcards=deck_getNCurrentCards(deck);
  for(i=0;i<N_RANKS;i++){
    rankcards=deck_getNCardsofRank(deck, (i+1));
    listprob[i]=(double)rankcards/totalcards;
  }

  return listprob;
}

int probability_countingHiLo(int rank){
  if(rank<1 || rank>10){
    fprintf(stderr, "probability_countingHiLo: rank<1 or rank>10, return -2\n");
    return -2;
  }
  if(rank>=2 && rank<=6){
    return +1;
  }
  else if(rank>=7 && rank<=9){
    return 0;
  }
  else{
    return -1;
  }
}

int probability_countingUstonSS(int rank){
  if(rank<1 || rank>10){
    fprintf(stderr, "probability_countingUstonSS: rank<1 or rank>10, return -3\n");
    return -3;
  }
  if(rank==2 || rank==3 || rank==4 || rank==6){
    return 2;
  }
  else if(rank==5){
    return 3;
  }
  else if(rank==7){
    return 1;
  }
  else if(rank==8){
    return 0;
  }
  else if(rank==9){
    return -1;
  }
  else{
    return -2;
  }
}
