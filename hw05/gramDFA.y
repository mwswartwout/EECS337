/*******************************************************************************
*
* FILE:		gramDFA.y
*
* DESC:		EECS 337 Assignment 5
*
* AUTHOR:	mws85
*
* DATE:		October 1, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
%{

#include	"yystype.h"

%}

%start line

%%

// DFA 
line		: A0 '\n'
		| line A0 '\n'
		| error '\n' { yysync(); yyerrok; }
		;
A0		: aletter A1
		| bletter A1
		| /* empty */ { printf( "DFA A0: accept\n"); }
		;
A1		: aletter A2
		| bletter A2
		;
A2		: aletter A3
		| bletter A3
		;
A3		: aletter A5
		| bletter A4
		| /* empty */ { printf( "DFA A3: accept\n"); }
		;
A4		: aletter A0
		| bletter A5
		| /* empty */ { printf( "DFA A4: accept\n"); }
		;
A5		: /* empty */ { printf( "DFA A5: accept\n"); }
		; 
aletter		: 'a'
		;
bletter		: 'b'
		;

%%

void	yyerror( char *s)
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", data.column, "^", data.column, s);
}
