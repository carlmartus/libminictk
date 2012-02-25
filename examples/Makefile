CC=cc
FLAGS=-Wall -O2 -lminictk
EXES=observer1

.PHONY: all clean

all: $(EXES)

%: %.c
	$(CC) -o $@ $^ $(FLAGS)

clean:
	$(RM) $(EXES)

