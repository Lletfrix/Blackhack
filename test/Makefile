CC=gcc
CFLAGS=-Wall

TESTS=test_deck.out
all: $(TESTS)

run_tests: $(TESTS)
	@./test_deck.out
	@echo ""

test_deck.out: test_deck.c tester.o
	$(CC) $(CFLAGS) -o test_deck.out test_deck.c ../deck.o tester.o

tester.o: tester.h tester.c
	$(CC) $(CFLAGS) -c tester.c

.PHONY: clean
clean:
	rm -rf *.o *.out
