/*******************************************************************************
*
* FILE:		second.y 
*
* DESC:		EECS 337 Assignment 6
*      		yacc program for the Calculator page 295-296
*
* AUTHOR:	caseid
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
%{
#include <ctype.h>
#include <stdio.h>
#define	 YYSTYPE	double	/* double type for yacc stack */
%}

%token NUMBER

%left '+' '-'
%left '*' '/' 
%right UMINUS /* supplies precedence for unary minus */

%% 	/* beginning of rules section */

lines	: lines expr '\n' { printf("%g\n", $2); }
	| lines '\n' 
	| /* empty */
	| error '\n' { yyerror(" reenter previous line: "); yyerrok; }
	;
expr	: expr '+' expr	{ $$ = $1 + $3;	}
	| expr '-' expr	{ $$ = $1 - $3;	}
	| expr '*' expr	{ $$ = $1 * $3;	}
	| expr '/' expr	{ $$ = $1 / $3;	}
	| '(' expr ')'	{ $$ = $2; }
	| '-' expr %prec UMINUS { $$ = - $2; }
	| number
	;

number	: NUMBER
	;
%%
#include "lex.yy.c"
