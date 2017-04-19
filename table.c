#include "table.h"

#define MAX_PLAYERS 10

struct _Table {
    Crupier *crupier;
    Deck *deck;
    Player *players[MAX_PLAYERS];
    int nPlayers;
};

bool table_isWorking(Table *t);

Table *table_ini()
{
    // use calloc to set all pointers to NULL;
    return calloc(1, sizeof(Table));
}

void table_destroy(Table *t)
{
    if (!t) return;

    crupier_destroy(t->crupier);
    deck_destroy(t->deck);

    // free players
    for (int i = 0; i < t->nPlayers; i++)
        player_destroy(t->players[i]);

    free(t);
    return;
}

Table *table_addPlayer(Table *t, Player *p)
{
    if (!t || !p) return NULL;

    t->players[t->nPlayers++] = p;
    return t;
}

Deck *table_getDeck(const Table *t)
{
    if (!t) return NULL;

    return t->deck;
}

Table *table_setDeck(Table *t, Deck *d)
{
    if (!t || !d) return NULL;

    t->deck = d;
    return t;
}

Crupier *table_getCrupier(const Table *t)
{
    if (!t) return NULL;

    return t->crupier;
}

Table *table_setCrupier(Table *t, Crupier *c)
{
    if (!t || !c) return NULL;

    t->crupier = c;
    return t;
}

Player **table_getPlayers(const Table *t)
{
    if (!t) return NULL;

    Player **players = malloc(sizeof(Player *) * t->nPlayers);
    for (int i = 0; i < t->nPlayers; i++)
        players[i] = t->players[i];

    return players;
}

int table_getNPlayers(const Table *t)
{
    return (!t) ? -1 : t->nPlayers;
}

Player *table_getPlayer(const Table *t, int player_index)
{
    if (!t || player_index < 0 || player_index >= t->nPlayers) return NULL;
    return t->players[player_index];
}

Table *table_makeBets(Table *t)
{
    if (!t) return NULL;

    for (int i = 0; i < t->nPlayers; i++){
        t->players[i]=player_bet(t->players[i], t);
        if(!t->players[i]){
            fprintf(stderr, "table_makeBets: ERROR in player_bet.\n");
            return NULL;
        }
    }

    return t;
}

Table *table_makePlays(Table *t)
{
    if (!t) return NULL;

    for (int i = 0; i < t->nPlayers; i++)
        player_play(t->players[i], t);

    return t;
}

Table *table_dealCard(Table *t)
{
    if (!table_isWorking(t)) return NULL;

    int cardAux;
    for (int i = 0; i < t->nPlayers; i++) {
        cardAux = deck_draw(t->deck);

        if (cardAux == -1) {
            fprintf(stderr, "cardAux failure\n");
            return NULL;
        }
        t->players[i] = player_addCardToHand(t->players[i], 0, cardAux);
    }

    return t;
}

Table *table_dealCardToCrupier(Table *t)
{
    if (!table_isWorking(t)) return NULL;

    int cardAux = deck_draw(t->deck);
    if (cardAux == -1) {
        fprintf(stderr, "cardAux failure\n");
        return NULL;
    }

    t->crupier = crupier_addCard(t->crupier, cardAux);
    return t;
}

Table *table_distributeEarnings(Table *t)
{
    //TODO: ERROR HANDLING

    Peg* data, thisGame;
    Hand* hAux;
    if (!table_isWorking(t)) return NULL;

    //Para cada jugador
    for(int i=0;i<t->nPlayers;i++){
        int value=0;
        data=player_handsCondition(t->crupier,t->players[i]);
        hAux=player_getHand(t->players[i], 0);
        //Se paga a cada mano
        int *hand_values_aux = hand_getValues(hAux);
        if((*hand_values_aux==21)&&hand_getNumCards(hAux)==2){
            //El jugador tiene BlackJack, no se recorrerán las manos
            player_addCash(t->players[i],2.5*player_getLastBet(t->players[i]));
            thisGame=WIN;
            value++;
        }
        else{
            //Se recorren las manos, y se va balanceando el valor value
            for(int j=0;j<player_getNHands(t->players[i]);j++){
                if(data[j]==WIN){
                    player_addCash(t->players[i],2*player_getLastBet(t->players[i]));
                    value++;
                }
                else if (data[j]==TIE){
                    player_addCash(t->players[i],player_getLastBet(t->players[i]));
                }
                else{
                    value--;
                }
            }

        }
        free(hand_values_aux);
        //Se añaden las estadisticas
        if(value>0) {
            thisGame=WIN;
        }
        else if(value<0){
            thisGame = LOSE;
        }
        else {
            thisGame=TIE;
        }
        player_addGame(t->players[i], thisGame);
        player_refreshStreak(t->players[i], thisGame);
        free(data);
    }
    return t;
}

/*
 * Esta función comprueba que la mesa está lista para jugar e informa apropiadamente de los errores que ocurran.
 * TODO: decidir dónde y cuántas veces llamamos a esta función, porque ralentiza un poco la ejecución.
 */
bool table_isWorking(Table *t)
{
    if (!t)
    {
        fprintf(stderr, "table not working: table is NULL\n");
        return false;
    }

    if (!t->crupier)
    {
        fprintf(stderr, "table not working: crupier is NULL\n");
        return false;
    }

    if (!t->deck)
    {
        fprintf(stderr, "table not working: deck is NULL\n");
        return false;
    }

    for (int i = 0; i < t->nPlayers; i++)
    {
        if (!t->players[i])
        {
            fprintf(stderr, "table not working: some players are NULL\n");
            return false;
        }
    }

    return true;
}

Table *table_restartTable(Table * t){
    int i;
    Hand* hCrupier;
    if(!t){
        fprintf(stderr, "table_restartHands: invalid arguments.\n");
        return NULL;
    }
    for (i=0; i<t->nPlayers;i++){
        t->players[i]=player_restartHands(t->players[i]);
        if(!t->players[i]){
            fprintf(stderr, "table_restartHands: error restarting player hands\n");
            return NULL;
        }
    }
    hCrupier=crupier_getHand(t->crupier);
        if(!hCrupier){
            fprintf(stderr, "table_restartHands: error getting crupier hand\n");
        }
    hCrupier=hand_restartHand(hCrupier);
    if(!hCrupier){
            fprintf(stderr, "table_restartHands: error restarting crupier hand\n");
        }

        // if less than MIN_DECK_WEIGHT decks are remaining, reshuffle
    if (deck_getNCurrentCards(t->deck) < MIN_DECK_WEIGHT * N_CARDS_DECK){
        deck_restartDeck(t->deck);
    }

        return t;
}

int table_printPlayersPercentages(FILE *pf, Table *t){
  int nCharPrinted=0;

  if(!pf || !t){
    fprintf(stderr, "table_printPlayersPercentages: error FILE or Table pointing null\n");
    return -1;
  }

  for(int i=0; i<t->nPlayers; i++){
    nCharPrinted+=fprintf(pf, "Player %d:\n", i);
    nCharPrinted+=fprintf(pf, "\tWin Ratio: %lf %%\n", player_getWinRatio(t->players[i]));
    nCharPrinted+=fprintf(pf, "\tTie Ratio: %lf %%\n", player_getTieRatio(t->players[i]));
    nCharPrinted+=fprintf(pf, "\tLost Ratio: %lf %%\n", player_getLostRatio(t->players[i]));
  }

  return nCharPrinted;
}

int table_printPlayersMoney(FILE *pf, Table* t){
    int nCharPrinted=0;
    for (int i=0;i<t->nPlayers; i++){
        nCharPrinted+=fprintf(pf, "Player %d : %ld\n",i, player_getCash(t->players[i]));
    }
    return nCharPrinted;
}

int table_printPlayersStreakData (FILE *pf, Table*t){
    int nCharPrinted=0;
    for(int i=0; i<t->nPlayers;i++){
        nCharPrinted+=fprintf(pf, "Player %d: MaxWinStreak: %d, MaxTieStreak: %d, MaxLoseStreak: %d.\n",i,player_getMaxWinStreak(t->players[i]),player_getMaxTieStreak(t->players[i]),player_getMaxLoseStreak(t->players[i]));
    }
    return nCharPrinted;
}

int table_printLastGame (FILE *pf, Table*t, int numGame){
    int nCharPrinted=0;
    nCharPrinted+=fprintf(pf, "Game: %d\n",numGame);
    for(int i=0; i<t->nPlayers;i++){
        if (WIN==player_getLastPlay(t->players[i])){
            nCharPrinted+=fprintf(pf, "\tPlayer %d: WIN\n",i);
            nCharPrinted+=fprintf(pf, "\t\t Current cash: %ld\n",player_getCash(t->players[i]));
            continue;
        }
        if(TIE==player_getLastPlay(t->players[i])){
            nCharPrinted+=fprintf(pf, "\tPlayer %d: TIE\n",i);
            nCharPrinted+=fprintf(pf, "\t\t Current cash: %ld\n",player_getCash(t->players[i]));
            continue;
        }
        if(LOSE==player_getLastPlay(t->players[i])){
            nCharPrinted+=fprintf(pf, "\tPlayer %d: LOSE\n",i);
            nCharPrinted+=fprintf(pf, "\t\t Current cash: %ld\n",player_getCash(t->players[i]));
            continue;
        }
    }
    return nCharPrinted;
}
