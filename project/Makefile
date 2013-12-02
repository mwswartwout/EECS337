#-----------------------------------------------------------------------------
#
# FILE:		Makefile
#
# DESC:		EECS 337 Project
#
# AUTHOR:	caseid
#
# DATE:		December 5, 2013
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
all:		ansi_c 

SRC	= gram.y scan.l main.c audit.c tuple.c symbols.c codegen.c 
OBJ	= gram.o scan.o main.o audit.o tuple.o symbols.o codegen.o 

ansi_c :	$(OBJ)
	$(LINK) $(CFLAGS) $(OBJ) -o ansi_c

scan.o	: y.tab.h 

clean	:
	rm -f gram.c y.tab.h y.output *.o

totar	:
	tar cvf project_caseid.tar Makefile project_test.sh main.c scan.l gram.y yystype.h tokens.h audit.c tuple.c symbols.c codegen.c test1.c test2.c test3.c test3.txt mustang.c gram.diff scan.diff

fromtar	:
	tar xvf project_caseid.tar 

