CC=cc
FLAGS=-Wall -O2 -lminictk
EXES=dbuf1 observer1 ptree1

.PHONY: all clean

all: $(EXES)

%: %.c
	$(CC) -o $@ $^ $(FLAGS)

clean:
	$(RM) $(EXES)

