LIBNAME=minictk
LIBFILE=lib$(LIBNAME).a

CC=cc
CFLAGS=-Wall -O2 -g3
DOCDIR=doc

_OBJ=minictk mem vectors dbuf
ODIR=obj
OBJ=$(_OBJ:%=$(ODIR)/%.o)

.PHONY: all info doc clean re

all: $(LIBFILE)

$(LIBFILE): $(OBJ)
	ar -cvq $(LIBFILE)

info: $(LIBFILE)
	ar -t $(LIBFILE)

# ar -cvq libctest.a ctest1.o ctest2.o
# List files in library: ar -t libctest.a

install: $(LIBFILE)
	@echo "TODO: Install library to /usr/lib"

doc:
	doxygen

$(ODIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(LIBFILE) $(OBJ) -r $(DOCDIR)/*

re: clean all

