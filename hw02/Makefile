#-----------------------------------------------------------------------------
#	Makefile
#	EECS 337 Compilers Fall 2013
#	by dxo4@case.edu
#
# REVISION HISTORY
#
#-----------------------------------------------------------------------------
.KEEP_STATE:
SHELL=/bin/bash

#
#	c compiler and linker
#
CC=	gcc
LINK=	gcc
LEX=	flex


#	yacc lex compiler linker flags
#
YFLAGS	= -dv
CFLAGS	= -g
LDFLAGS	= -g 
#
#	linker
#
LIBRARIES=	-lm

#
#	to make all
#
all:		Echo2

#
#	Echo2
#
ECHO2_SRCS	= Echo2.c
ECHO2_OBJS	= $(ECHO2_SRCS:%.c=%.o)

Echo2:	$(ECHO2_OBJS)
	$(LINK) $(ECHO2_OBJS) $(LIBRARIES) $(LDFLAGS) -o Echo2

#
#	clean
#
clean:
		-rm -f *.o lex.c yacc.c y.output
