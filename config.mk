PROG    = sxbg
VERSION = 1.0

CC     = cc
PREFIX = /usr/local
MANDIR = /share/man
BINDIR = /bin

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS   = -std=c99 -Wpedantic -Wall -Wextra -O2
LDFLAGS  = -s
LDLIBS   = -lxcb -lxcb-util
