CC=gcc
CFLAGS=-Werror

TESTS=test_deck

test: clean $(TESTS)
	@./test_deck 2>/dev/null

minitest.o: minitest.c
	$(CC) $(CFLAGS) -c minitest.c

test_deck: test_deck.c minitest.o
	$(CC) $(CFLAGS) -o test_deck test_deck.c ../deck.o minitest.o

.PHONY: clean
clean:
	rm -rf $(TESTS)
