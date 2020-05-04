#!/usr/bin/env make

################
# tool locations
################
#
SHELL= /bin/bash
CC= gcc
CFLAGS=
PROJNAME= samp-git-log

all: $(PROJNAME).c
		$(CC) $(CFLAGS) $(PROJNAME).c -o $(PROJNAME)

debug: $(PROJNAME).c
		$(CC) $(CFLAGS) -Wall -g -O0 $(PROJNAME).c -o $(PROJNAME)

install: all
		@echo are you kidding'??'

clean:
		rm $(PROJNAME)
