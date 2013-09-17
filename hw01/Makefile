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
all:		Code_1_6_1 Code_1_6_2 Code_1_6_4 Echo Echo2

#
#	Code_1_6_1
#
CODE_1_6_1_SRCS	= Code_1_6_1.c
CODE_1_6_1_OBJS	= $(CODE_1_6_1_SRCS:%.c=%.o)

Code_1_6_1:	$(CODE_1_6_1_OBJS)
		$(LINK) $(CODE_1_6_1_OBJS) $(LIBRARIES) $(LDFLAGS) -o Code_1_6_1

#
#	Code_1_6_2
#
CODE_1_6_2_SRCS	= Code_1_6_2.c
CODE_1_6_2_OBJS	= $(CODE_1_6_2_SRCS:%.c=%.o) 

Code_1_6_2:	$(CODE_1_6_2_OBJS)
		$(LINK) $(CODE_1_6_2_OBJS) $(LIBRARIES) $(LDFLAGS) -o Code_1_6_2

#
#	Code_1_6_4
#
CODE_1_6_4_SRCS	= Code_1_6_4.c
CODE_1_6_4_OBJS	= $(CODE_1_6_4_SRCS:%.c=%.o) 

Code_1_6_4:	$(CODE_1_6_4_OBJS)
		$(LINK) $(CODE_1_6_4_OBJS) $(LIBRARIES) $(LDFLAGS) -o Code_1_6_4

#
#	Echo
#
ECHO_SRCS	= Echo.c
ECHO_OBJS	= $(ECHO_SRCS:%.c=%.o)

Echo:	$(ECHO_OBJS)
	$(LINK) $(ECHO_OBJS) $(LIBRARIES) $(LDFLAGS) -o Echo

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
