#-----------------------------------------------------------------------------
#
# FILE:		Makefile
#
# DESC:		EECS 337 Assignment 5
#
# AUTHOR:	caseid
#
# DATE:		October 1, 2013
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

#
#	to make all
#
all:		hw05testNFA hw05testDFA

SRCNFA	= gramNFA.y scan.l main.c
OBJNFA	= gramNFA.o scan.o main.o

hw05testNFA :	$(OBJNFA)
	$(LINK) $(CFLAGS) $(OBJNFA) -o hw05testNFA
	cp y.output yNFA.output

SRCDFA	= gramDFA.y scan.l main.c
OBJDFA	= gramDFA.o scan.o main.o

hw05testDFA :	$(OBJDFA)
	$(LINK) $(CFLAGS) $(OBJDFA) -o hw05testDFA
	cp y.output yDFA.output

scan.o	: y.tab.h

clean	:
	rm -f scan.c gram.c y.tab.h y.output *.o

totar	:
	tar cvf hw05_caseid.tar Makefile hw05_test.sh gramNFA.y gramDFA.y main.c scan.l test1.c test2.c test3.c yystype.h

fromtar	:
	tar xvf hw05_caseid.tar 

