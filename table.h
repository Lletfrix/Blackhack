#include "deck.h"
#include "crupier.h"
#include "player.h"

typedef struct _Table Table;


/*
 * Returns a new table. */
Table *table_ini();

/*
 * Frees a table and all children attributes (Deck, Players, Crupier and their Hands).
 */
void table_destroy(Table *);

/*
 * Stores a pointer to the specified player in table.
 */
Table *table_addPlayer(Table *, Player *);

/*
 * Returns a pointer to the deck of the game. Modifications to the deck will affect the rest of the deck.
 */
Deck *table_getDeck(const Table *);

/*
 * Sets the deck of the table to the specified Deck.
 */
Table *table_setDeck(Table *, Deck *);

/*
 * Returns a pointer to the crupier. Modifications to the crupier will affect the rest of the game.
 */
Crupier *table_getCrupier(const Table *);

/*
 * Sets the crupier of the game to the specified one.
 */
Table *table_setCrupier(Table *, Crupier *);

/*
 * Returns an array of pointers to Players. Modifications to players will affect the rest of the game.
 */
Player **table_getPlayers(const Table *);

/*
 * Returns the number of players on the table.
 */
int table_getNPlayers(const Table *);

/*
 * Returns the ith player on the table. Modifications to that player will affect the rest of the game.
 */
Player *table_getPlayer(const Table *, int player_index);
