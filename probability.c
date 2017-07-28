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

double *probability_goOver(Deck *d, Hand *h){
  int *handValues, value0, value1, i;
  double *listofprob, *probabilities;

  if(!d || !h){
    fprintf(stderr, "probability_goOver: error arguments pointing NULL\n");
    return NULL;
  }
  handValues=hand_getValues(h);
  listofprob=probability_listofcards(d);
  value0=handValues[0];
  value1=handValues[1];

  probabilities=(double*)malloc(2*sizeof(double));

  probabilities[0]=0;
  probabilities[1]=0;

  if(value0==21){
    probabilities[0]=1;
  }
  if(value0==20){
    probabilities[0]=1-listofprob[0];
  }
  if(value0==19){
    for(i=0;i<2;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }
  if(value0==18){
    for(i=0;i<3;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }
  if(value0==17){
    for(i=0;i<4;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==16){
    for(i=0;i<5;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==15){
    for(i=0;i<6;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==14){
    for(i=0;i<7;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==13){
    for(i=0;i<8;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==12){
    for(i=0;i<9;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0==11){
    for(i=0;i<10;i++){
      probabilities[0]+=listofprob[i];
    }
    probabilities[0]=1-probabilities[0];
  }

  if(value0<11){
    probabilities[0]=0;
  }

  /*************************/

  if(value1==21){
    probabilities[1]=1;
  }
  if(value1==20){
    probabilities[1]=1-listofprob[0];
  }
  if(value1==19){
    for(i=0;i<2;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }
  if(value1==18){
    for(i=0;i<3;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }
  if(value1==17){
    for(i=0;i<4;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1==16){
    for(i=0;i<5;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1==15){
    for(i=0;i<6;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1==14){
    for(i=0;i<7;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1==13){
    for(i=0;i<8;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1==12){
    for(i=0;i<9;i++){
      probabilities[1]+=listofprob[i];
    }
    probabilities[1]=1-probabilities[1];
  }

  if(value1<12){
    probabilities[1]=0;
  }

  return probabilities;
}
