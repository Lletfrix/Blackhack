#include "minitest.h"
#include <unistd.h>

#include "../deck.h"

void deck_visualize_extraction(unsigned seed);

int main(int argc, char **argv) {
    unsigned seed = time(NULL);
    printf("\nThis test run's seed is %u.\n", seed);

    printf("Testing a deck...\n");

    Deck *d;
    assert("can initialize a deck",
            (d = deck_ini(seed)));

    int n_cards = 0;
    assert("deck has some cards inside",
            n_cards = deck_getNCurrentCards(d));

    bool can_draw_all_cards = true;
    int card_value_sum = 0, card;
    for (int i = 0; i < n_cards; i++) {
        card = deck_draw(d);
        if (card < 0) {
            can_draw_all_cards = false;
            break;
        }

        card_value_sum += card;
    }
    assert("can draw as many cards as there are in a deck",
            can_draw_all_cards);

    assert("can't draw any more cards",
            deck_draw(d) < 0);

    assert("draw all cards and they add up to the whole deck",
            card_value_sum == N_DECKS * 85 * 4);

    assert("when deck is empty, it has no cards",
            deck_isEmpty(d) && deck_getNCurrentCards(d) == 0);

    assert("deck can be reinitialized",
            d = deck_restartDeck(d));

    d = deck_removeCard(d, 10);
    assert("drawing a card updates running count",
            deck_runningCount(d) == -1);

    assert("can also retrieve real count",
            round(deck_realCount(d)) == round(-1.0 / N_DECKS));

    assert("deck has some cards after reinit",
            !deck_isEmpty(d));

    d = deck_restartDeck(d);
    bool correct_distribution = true;
    double *dist = deck_distribution(d);
    double expected = 4.0 / N_CARDS_DECK;
    for (int i = 0; i < N_RANKS - 1; i++) {
        if (dist[i] != expected) {
            printf("incorrect distribution for %d, should be %lf, but is %lf instead\n", i, expected, dist[i]);
            correct_distribution = false;
        }
    }
    free(dist);
    assert("can give a list of the distribution of remaining cards",
            correct_distribution);

    deck_destroy(d);
    assert("can destroy a deck", true);

    d = deck_ini(seed);
    printf("drawing some cards, check your randomness...\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", deck_draw(d));
    }
    printf("\n");
    deck_destroy(d);

    if (argc == 2) {
        printf("visualize drawing cards until deck is empty");
        deck_visualize_extraction(seed);
    }


    return 0;
}

void print_stack(double intensity, char sym) {
    printf("%c: %6.2lf%%: ", sym, intensity * 100);
    for (int i = 0; i < intensity * 250; i++)
        printf("#");

    printf("\n");
}

void deck_visualize_extraction(unsigned seed) {
    Deck *d;
    int card;
    double *dist;
    d = deck_ini(seed);
    while ((card = deck_draw(d)) != -1) {
        printf("drew a(n) %d\n", card);
        dist = deck_distribution(d);
        print_stack(dist[0], 'A');
        for (int i = 1; i < N_RANKS - 1; i++) {
            print_stack(dist[i], '0' + i + 1);
        }
        print_stack(dist[9], 'T');

        free(dist);
        usleep(50000);
    }

    deck_destroy(d);
}
