LIBS   = -lX11
CFLAGS := -std=c99 -g -Wall -Wextra -Wno-unused-parameter
PREFIX ?= $(HOME)
BINDIR ?= $(PREFIX)/bin
CC     ?= gcc

all: nfyer

nfyer: nfyer.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

install: all
	install -Dm755 nfyer $(DESTDIR)$(BINDIR)/nfyer
	install -Dm755 nfy $(DESTDIR)$(BINDIR)/nfy

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/nfyer

clean:
	rm -f nfyer *.o

.PHONY: all install uninstall clean
