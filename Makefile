LIBNAME=minictk
#LIBFILE=lib$(LIBNAME).a
LIBFILE=lib$(LIBNAME).so
LIBHEAD=minictk.h

CC=cc
CFLAGS=-Wall -O2 -fPIC -g3
DOCDIR=doc

_OBJ=main mem vectors dbuf observer ptree
ODIR=obj
OBJ=$(_OBJ:%=$(ODIR)/%.o)

CP=cp
RM=rm -f

.PHONY: all info doc clean re install clean-all

all: $(LIBFILE)

$(LIBFILE): $(OBJ)
	ar -cvq $(LIBFILE) $(OBJ)
#$(CC) -shared -Wl,-soname,$(LIBFILE) -o $(LIBFILE) $^

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

