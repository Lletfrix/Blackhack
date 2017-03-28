#include "tester.h"
#include "../deck.h"

int main(void)
{
    printf("Testing a Deck...\n");

    Deck *d = NULL;

    assert("can initialize a deck",
            (d = deck_ini()));

    int n_cards = 0;
    assert("deck has some cards inside",
            n_cards = deck_getNCurrentCards(d));

    bool can_draw_all_cards = true;
    for (int i = 0; i < n_cards; i++)
        can_draw_all_cards = (deck_draw(d) > 0) ? true : false;
    assert("can draw as many cards as there are in a deck",
            can_draw_all_cards);

    assert("can't draw any more cards",
            deck_draw(d) < 0);

    assert("when deck is empty, it has no cards",
            deck_isEmpty(d) && deck_getNCurrentCards(d) == 0);

    assert("deck can be reinitialized",
            d = deck_restartDeck(d));

    assert("deck has some cards after reinit",
            !deck_isEmpty(d));

    deck_destroy(d);
    assert("can destroy a deck", true);

    return 0;
}
