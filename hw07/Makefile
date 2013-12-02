#-----------------------------------------------------------------------------
#
# FILE:		Makefile
#
# DESC:		EECS 337 Assignment 6
#
# AUTHOR:	caseid
#
# DATE:		October 8, 2013
#
# EDIT HISTORY:	
#
#		Updated for EECS 337 Assignment 7 October 15, 2013 
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
all:		calc 

SRC	= second.y first.l calc.c
OBJ	= second.o first.o calc.o

calc :	$(OBJ)
	$(LINK) $(CFLAGS) $(OBJ) -lm -o calc

clean	:
	rm -f first.c second.c y.tab.h y.output *.o

totar	:
	tar cvf hw07_caseid.tar book/* Makefile hw07_test.sh yystype.h first.l second.y calc.c math10.txt math11.txt math12.txt math13.txt math14.txt math15.txt math16.txt math17.txt math18.txt math19.txt 

fromtar	:
	tar xvf hw07_caseid.tar 
