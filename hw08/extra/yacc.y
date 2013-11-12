/*******************************************************************************
*
* FILE:		yacc.y (second.y)
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
*		Updated for EECS 337 Assignment 7 October 15, 2013 
*		Updated for EECS 337 Assignment 8 October 29, 2013 
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
%right UMINUS UPLUS '~' /* supplies precedence for unary minus */

%% 	/* beginning of rules section */

lines	: lines stmts '\n' 
	{
/*
 *	print the quad list
 */
		print_quad_list( $2.quad);
/*
 *	free the quad list
 */
		free_quad_list( $2.quad);
	}
	| lines error '\n' { yyerror(" reenter previous line: "); yyerrok; }
	| /* empty */
	;
stmts	: expr 
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
/* 	add bitwise operators */
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
	| '(' expr ')'	
	{
		$$ = $2;
	}
	| '~' expr 
	{
		$$.quad = new_quad2( '~', $2.quad);
	}
	| '-' expr %prec UMINUS 
	{
		$$.quad = new_quad2( UMINUS, $2.quad);
	}
	| '+' expr %prec UPLUS
	{
		$$.quad = new_quad2( UPLUS, $2.quad);
	}
	| number
	{
		$$.quad = new_quad3( '=', $1.index, 0);
	}
	| ident
	{
		$$.quad = new_quad3( '=', $1.index, 0);
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

