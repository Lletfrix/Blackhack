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

    for (int i = 0; i < t->nPlayers; i++)
        player_bet(t->players[i], t);

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
            fprintf(stderr, "cardAux failure");
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
        fprintf(stderr, "cardAux failure");
        return NULL;
    }

    t->crupier = crupier_addCard(t->crupier, cardAux);
    return t;
}

Table *table_distributeEarnings(Table *t)
{
    if (!table_isWorking(t)) return NULL;

    Hand *crupierHand = crupier_getHand(t->crupier);

    // de momento asumimos que cada jugador solo tiene una mano
    for (int i = 0; i < t->nPlayers; i++) {
        Hand *playerHand = player_getHand(t->players[i], 0);
        int compareValue=(int) hand_compare(crupierHand, playerHand);
        printf("player %d, %d\n", i, compareValue);
        player_addGame(t->players[i], compareValue);
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
