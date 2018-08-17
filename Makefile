#!/usr/bin/env make

################
# tool locations
################
#
SHELL= /bin/bash
CC= gcc
CFLAGS=

all: samp-git-log.c
		$(CC) $(CFLAGS) samp-git-log.c -o samp-git-log

install: all
		@echo are you kidding'??'

clean:
		rm git-hash