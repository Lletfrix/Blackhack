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
    /* for each player */
    for (int i = 0; i < t->nPlayers; i++){
        /* for each hand */
        for (int j = 0; j < player_getNHands(t->players[i]); j++){
            t->players[i]=player_bet(t->players[i], t);
            if(!t->players[i]){
                fprintf(stderr, "table_makeBets: ERROR in player_bet.\n");
                return NULL;
            }
        }
    }

    return t;
}

Table *table_makePlays(Table *t)
{
    if (!t) return NULL;

    for (int i = 0; i < t->nPlayers; i++)
        if(NULL==player_play(t->players[i], t)) return NULL;

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

Table *table_distributeEarnings(Table *t){
    Player* p, *pErr;
    Hand* h, *hCrupier;
    Peg condition;
    int* value;
    int numHands=-1, i, j;
    hCrupier=crupier_getHand(t->crupier);
    /* for each player*/
    for (i=0; i<t->nPlayers;i++){
        p=t->players[i];
        numHands=player_getNHands(p);
        /* for each hand */
        for (j=0; j<numHands; j++){
            h=player_getHand(p, j);
            value=hand_getValues(h);
            if( value[0]==21 && hand_getNumCards(h)==2){
                /*pErr=player_addCash(p, 2.5*hand_getCurrentBet(h)); no hace falta, se paga en la ejecución de la función "play" en cuestion*/
                player_addGame(p, WIN);
                player_blackJack(p);
                player_refreshStreak(p, WIN);
                continue;
            }
            condition=hand_compare(hCrupier, h);
            if (condition==LOSE){
                player_addGame(p, LOSE);
                player_refreshStreak(p, LOSE);
            }
            else if (condition == TIE){
                pErr=player_addCash(p, hand_getCurrentBet(h));
                player_addGame(p, TIE);
                player_refreshStreak(p, TIE);
            }
            else if (condition == WIN){
                pErr=player_addCash(p, 2*hand_getCurrentBet(h));
                player_addGame(p, WIN);
                player_refreshStreak(p, WIN);
            }
            else{
                return NULL;
            }
        }
    }
    if (!pErr) return NULL;
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
    nCharPrinted+=fprintf(pf, "\nPlayer: %d\tHands played: %d\tBlackjacks obtained: %d\n", i, player_getNPlayed(t->players[i]), player_getNBlackJack(t->players[i]));
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

int table_printCash(FILE *fp, Table *t){
    int nCharPrinted=0;
    if(!fp || !t){
        return -1;
    }
    for (size_t i = 0; i < t->nPlayers; i++) {
        nCharPrinted+=fprintf(fp, "%ld", player_getCash(t->players[i]));
        if(i == table_getNPlayers(t)-1) nCharPrinted+=fprintf(fp, "\n");
        else nCharPrinted+=fprintf(fp, " ");
    }
    return nCharPrinted;
}
