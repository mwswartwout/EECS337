#-----------------------------------------------------------------------------
#
# FILE:		Makefile
#
# DESC:		EECS 337 Assignment 4
#
# AUTHOR:	caseid
#
# DATE:		September 17, 2013
#
# EDIT HISTORY:	
#
#-----------------------------------------------------------------------------
.KEEP_STATE:
SHELL=/bin/bash

#
#	define version of c compiler, linker and lex
#
CC=	gcc
LINK=	gcc
LEX=	lex
#
#	define yacc lex and compiler flags
#
YFLAGS	= -dv
LFLAGS	=
CFLAGS	= -g

SRC	= gram.y scan.l main.c attribute.c
OBJ	= gram.o scan.o main.o attribute.o

ansi_c :	$(OBJ)
	$(LINK) $(CFLAGS) $(OBJ) -o ansi_c

scan.o	: y.tab.h

clean	:
	rm -f scan.c gram.c y.tab.h y.output *.o

totar	:
	tar cvf hw04_caseid.tar hw04_test.sh Makefile attribute.c

fromtar	:
	tar xvf hw04_caseid.tar 
