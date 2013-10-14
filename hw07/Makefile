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
	tar cvf hw06_caseid.tar book/* Makefile hw06_test.sh yystype.h first.l second.y calc.c math0.txt math1.txt math2.txt math3.txt math4.txt math5.txt math6.txt math7.txt math8.txt math9.txt 

fromtar	:
	tar xvf hw06_caseid.tar 
