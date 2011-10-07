# log makefile
# 2011-09-14 19:12
# by Ian D Brunton <iandbrunton at gmail dot com>

APPNAME=log
VERSION=1.0
CC=gcc
CFLAGS=-c -g -Wall -DVERSION=\"$(VERSION)\" -DAPPNAME=\"$(APPNAME)\" \
	`pkg-config --cflags glib-2.0`
LDFLAGS=-Wl `pkg-config --libs glib-2.0`
SRC=main.c
OBJ=$(SRC:.c=.o)

all: $(APPNAME)

$(APPNAME): $(OBJ)
	$(CC) $(LDFLAGS) -Wl,--start-group $(OBJ) -Wl,--end-group -o $@

.c.o:
	$(CC) $(CFLAGS)  $< -o $@

clean:
	rm -rf *.o $(APPNAME)
