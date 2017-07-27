CC=gcc
CFLAGS=-Werror

TESTS=test_deck test_hand

run_tests: clean $(TESTS)
	@./test_deck 2>/dev/null
	@./test_hand

test_distribution:
	@./test_deck dist

minitest.o: minitest.c
	$(CC) $(CFLAGS) -c minitest.c

test_deck: test_deck.c minitest.o ../deck.o
	$(CC) $(CFLAGS) -o test_deck test_deck.c ../deck.o minitest.o

test_hand: test_hand.c ../hand.o ../deck.o
	$(CC) $(CFLAGS) -o test_hand test_hand.c ../hand.o minitest.o ../deck.o

.PHONY: clean
clean:
	rm -rf $(TESTS)
