CC     ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic -O2
CFLAGS += -std=c11

SANITIZE = -fsanitize=address,undefined -fno-omit-frame-pointer -g

.PHONY: all test clean

all: teatro

teatro: src/main.c src/teatro.c src/teatro.h
	$(CC) $(CFLAGS) src/main.c src/teatro.c -o $@

# Testes de unidade compilados com AddressSanitizer + UBSan (vazamentos e acessos inválidos).
test: tests/test_teatro.c src/teatro.c src/teatro.h
	$(CC) $(CFLAGS) $(SANITIZE) tests/test_teatro.c src/teatro.c -o test_teatro
	./test_teatro

clean:
	rm -f teatro test_teatro
