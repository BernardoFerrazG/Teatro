# Theatre Ticketing

[![CI](https://github.com/BernardoFerrazG/Teatro/actions/workflows/ci.yml/badge.svg)](https://github.com/BernardoFerrazG/Teatro/actions/workflows/ci.yml)

A terminal ticket-selling system for a theatre, written in C. It is an exercise in **dynamic memory management** and **abstract data types**. The theatre is a heap-allocated array of rows of seats, its size is chosen at runtime, and every allocation is freed. The interface is in Portuguese; this README is in English. The original assignment report is in [`relatorio.pdf`](relatorio.pdf).

```
Fila  1 | A A C D D
Fila  2 | B B - - -
Fila  3 | - - - - -
```

Each seat shows the initial of its buyer, or `-` if it is free.

## Features

- **Create a theatre** with any number of rows and seats per row. It can be replaced only while no ticket has been sold.
- **Sell tickets**: a buyer chooses a row and a number of seats. If the free seats in that row are scattered, the people already seated are **moved towards the start of the row** so the new group can sit together, as the assignment requires.
- **Return tickets**: frees every seat booked under a buyer's name.
- **Print the seating map.**

## Design

```
src/
├── teatro.h / teatro.c   # Theatre ADT: create, sell, return, compact, print — no user I/O
└── main.c                # Menu and input validation
tests/
└── test_teatro.c         # Unit tests for the ADT
```

- The theatre logic doesn't read from the keyboard, so it can be unit tested. All interaction lives in `main.c`.
- Input is read line by line with `fgets` and parsed with `strtol`. Letters, out-of-range numbers and overly long names are rejected or safely truncated, and they no longer break the menu.
- `Teatro` is passed by pointer, and `teatro_libera` is safe to call more than once.

## Build and run

Requires a C11 compiler and `make`.

```bash
make            # build ./teatro
./teatro
make test       # unit tests with AddressSanitizer + UndefinedBehaviorSanitizer
make clean
```

## Tests

`make test` builds the unit tests with **AddressSanitizer and UBSan**, so memory leaks and out-of-bounds accesses fail the build. The tests cover creation and invalid sizes, selling until a row is full, returns, the seat-compaction rule and long names. CI runs them, plus a `-Werror` build, on every push.

## Changes from the original version

- **Memory leak fixed**: replacing an empty theatre allocated a new one without freeing the old one.
- **Infinite loop fixed**: typing a letter in the menu made `scanf` fail forever and print "invalid option" in a loop.
- Buyer names were limited to a single character (`char nome[2]`) and read with a `getchar()` workaround. They can now have up to 31 characters.
- The `vazio` ("empty") flag meant the opposite of its name. It was renamed to `ocupado` ("occupied").
- Compiled binaries and object files were removed from the repository.
