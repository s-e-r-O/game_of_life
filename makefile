IDIR=./inc
CC=gcc
CFLAGS=-I$(IDIR)

SRC_M = src/master/*.c
SRC_S = src/slave/*.c

all: master slave

master: $(SRC_M)
	$(CC) -o $@ $^ $(CFLAGS)

slave: $(SRC_S)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f master *~ *.o
	rm -f slvae *~ *.o 