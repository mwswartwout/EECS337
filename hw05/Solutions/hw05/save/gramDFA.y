/*******************************************************************************
*
* FILE:		gramDFA.y
*
* DESC:		EECS 337 Assignment 5
*
* AUTHOR:	caseid
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
		| error '\n' { yysync(); yyerrok; }
		| line A0 '\n'
		;
A0		: aletter A1
		| bletter A4
		;
A1		: aletter A1
		| bletter A2
		;
A2		: aletter A3
		| bletter A5
		;
A3		: aletter A1
		| bletter A2
		| /* empty */ { printf( "DFA A3: accept\n"); }
		;
A4		: aletter A1
		| bletter A2
		;
A5		: aletter A3
		| bletter A5
		| /* empty */ { printf( "DFA A5: accept\n"); }
		;
aletter		: 'a'
		;
bletter		: 'b'
		;

%%

void	yyerror( char *s)
{
	printf("\n%*s\n%*s\n", data.column, "^", data.column, s);
	fflush(stdout);
}
