/*******************************************************************************
*
* FILE:		yacc.y 
*
* DESC:		EECS 337 Assignment 6
*      		yacc program for the Calculator page 295-296
*
* AUTHOR:	mws85
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
%{
#include <ctype.h>
#include <stdio.h>
#include "yystype.h"
#include "y.tab.h"
%}

%start lines

%token CONSTANT
%token IDENTIFIER

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%right UMINUS '~' /* supplies precedence for unary minus */

%% 	/* beginning of rules section */
lines	: lines stmts '\n'
	{
		print_quad_list( $2.quad);
		free_quad_list( $2.quad);
	} 
	| lines error '\n' { yyerror(" reenter previous line: "); yyerrok; }
	| 
	;
stmts 	: expr
	| ident '=' expr
	{
		$$.quad = new_quad3( '=', $1.index, $3.quad);
	}
	;
expr	: expr '+' expr	
	{
		$$.quad = new_quad1( '+', $1.quad, $3.quad);	
	}
	| expr '-' expr
	{
		$$.quad = new_quad1( '-', $1.quad, $3.quad);
	}	
	| expr '*' expr	
	{
		$$.quad = new_quad1( '*', $1.quad, $3.quad);
	}
	| expr '/' expr	
	{
		$$.quad = new_quad1( '/', $1.quad, $3.quad);
	}
	
	| '(' expr ')'
	{
		free_quad( $1.quad);	 
	}

	| expr '|' expr
	{
		$$.quad = new_quad1( '|', $1.quad, $3.quad);
	}
	| expr '^' expr
	{
		$$.quad = new_quad1( '^', $1.quad, $3.quad);
	}
	| expr '&' expr	
	{
		$$.quad = new_quad1( '&', $1.quad, $3.quad);
	}
	| expr '%' expr
	{
		$$.quad = new_quad1( '%', $1.quad, $3.quad);
	}
	| '~' expr
	{
		$$.quad = new_quad2( '~', $2.quad);
	}
	| number 
	| ident
	{
		$$ = symbol_tables[ $1.index].yylval;
	}
	;

number	: CONSTANT
	;

ident	: IDENTIFIER
	;
%%

void	yyerror( char *s)
{
	printf( "%s\n", s);
}

