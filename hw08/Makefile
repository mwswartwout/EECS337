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
#		Updated for EECS 337 Assignment 8 October 22, 2013 
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

SRC	= yacc.y lex.l main.c quad.c
OBJ	= yacc.o lex.o main.o quad.o

calc :	$(OBJ)
	$(LINK) $(CFLAGS) $(OBJ) -lm -o calc

clean	:
	rm -f lex.c yacc.c y.tab.h y.output *.o

totar	:
	tar cvf hw08_caseid.tar Makefile hw08_test.sh yystype.h main.c quad.c symbol_table.c math20.txt
#	tar cvf hw08_caseid.tar Makefile hw08_test.sh yystype.h lex.l yacc.y main.c quad.c symbol_table.c math20.txt

fromtar	:
	tar xvf hw08_caseid.tar 
