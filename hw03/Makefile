#-----------------------------------------------------------------------------
#
# FILE:		Makefile
#
# DESC:		EECS 337 Assignment 3
#
# AUTHOR:	caseid
#
# DATE:		September 10, 2013
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

SRC	= gram.y scan.l main.c
OBJ	= gram.o scan.o main.o

ansi_c :	$(OBJ)
	$(LINK) $(CFLAGS) $(OBJ) -o ansi_c

scan.o	: y.tab.h

clean	:
	rm -f scan.c gram.c y.tab.h y.output *.o

totar	:
	tar cvf hw03_caseid.tar hw03_test.sh Makefile main.c scan.l gram.y yystype.h tokens2.c original/c-grammar

fromtar	:
	tar xvf hw03_caseid.tar 
