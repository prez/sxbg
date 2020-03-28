# See LICENSE file for copyright and license details
.POSIX:
.SUFFIXES:

include config.mk

all: sxbg
sxbg.o: sxbg.c config.mk

sxbg: sxbg.o config.mk
	$(CC) -o $@ $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) sxbg.o $(LDLIBS)

dist:
	rm -rf "$(PROG)-$(VERSION)"
	mkdir "$(PROG)-$(VERSION)"
	cp -r LICENSE makefile sxbg.c $(PROG).1 \
		"$(PROG)-$(VERSION)"
	tar -cf - "$(PROG)-$(VERSION)" | gzip -c > "$(PROG)-$(VERSION).tar.gz"
	rm -rf "$(PROG)-$(VERSION)"

install: $(PROG)
	install -Dm 0755 "$(PROG)" "$(DESTDIR)$(PREFIX)$(BINDIR)"
	install -Dm 0644 "$(PROG).1" "$(DESTDIR)$(PREFIX)$(MANDIR)/man1"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(BINDIR)/$(PROG)"
	rm -f "$(DESTDIR)$(PREFIX)$(MANDIR)/man1/$(PROG).1"

clean:
	rm -f $(PROG) $(PROG).core $(PROG).su core sxbg.o

distclean: clean
	rm -f "$(PROG)-$(VERSION).tar.gz"

gitignore:
	rm -f .gitignore
	{ printf '*.o\n'; printf '%s\n' $(PROG) $(PROG).1.scd ; printf '.clang-format\n'; } > .$@

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<
