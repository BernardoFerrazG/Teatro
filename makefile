CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: teatro

teatro: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f teatro $(OBJS)