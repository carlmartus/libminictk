LIBNAME=minictk
LIBFILE=lib$(LIBNAME).a
LIBHEAD=minictk.h

CC=cc
CFLAGS=-Wall -O2 -g3
DOCDIR=doc

_OBJ=main mem vectors dbuf observer
ODIR=obj
OBJ=$(_OBJ:%=$(ODIR)/%.o)

CP=cp
RM=rm -f

.PHONY: all info doc clean re install clean-all

all: $(LIBFILE)

$(LIBFILE): $(OBJ)
	ar -cvq $(LIBFILE) $(OBJ)

info: $(LIBFILE)
	ar -t $(LIBFILE)

# ar -cvq libctest.a ctest1.o ctest2.o
# List files in library: ar -t libctest.a

install: $(LIBFILE)
	$(CP) $(LIBHEAD) /usr/include/
	$(CP) $(LIBFILE) /usr/lib/

doc:
	doxygen

$(ODIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) $(LIBFILE) $(OBJ)

clean-all: clean
	$(RM) -r $(DOCDIR)/*
	$(MAKE) -C examples clean

re: clean all

