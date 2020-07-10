LIBS   = -lX11
CFLAGS := -std=c99 -g -Wall -Wextra -Wno-unused-parameter
PREFIX ?= $(HOME)
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

all: nfy

nfy: nfy.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

install: all
	install -Dm755 nfy $(DESTDIR)$(BINDIR)/nfy

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/nfy

clean:
	rm -f nfy *.o

.PHONY: all install uninstall clean
