/*******************************************************************************
*
* FILE:		gramNFA.y
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

// NFA 
line		: A0 '\n'
		| error '\n' { yysync(); yyerrok; }
		| line A0 '\n'
		;
A0		: aletter A0
		| bletter A0
		| aletter A1
		| bletter A1
		;
A1		: bletter A2
		;
A2		: aletter A3
		| bletter A3
		;
A3		: /* empty */ { printf( "NFA A3: accept\n"); }
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
