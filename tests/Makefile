CC=cc

SRC=$(wildcard *.c)
LIBS=$(SRC:%.c=test_%.ef)
OBJS=$(wildcard ../obj/*.o)

.PHONY: all clean re

all: $(LIBS)

test_%.ef: %.c
	@echo "Building $@ from $<"
	@$(CC) -shared -fPIC -Wl,-soname,$@ -o $@ $< $(OBJS)

clean:
	$(RM) $(LIBS)

re: clean all

