IDIR=./inc
CC=gcc
CFLAGS=-I$(IDIR)
CLIBS=-lncurses
BDIR=./bin

SRC_M = src/master/*.c src/utils/connection/*.c src/utils/*.c
SRC_S = src/slave/*.c src/utils/connection/*.c src/utils/*.c

all: master slave

master: $(SRC_M)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(CLIBS)

slave: $(SRC_S)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) 

clean:
	rm -f $(BDIR)/master *~ *.o
	rm -f $(BDIR)/slave *~ *.o 