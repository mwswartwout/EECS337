/*******************************************************************************
*
* FILE:		second.y 
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

%token INTEGER
%token FLOAT
%token IDENTIFIER

%left '|'
%left '^'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%right UMINUS '~' /* supplies precedence for unary minus */

%% 	/* beginning of rules section */
lines	: lines expr '\n' 
	{
		switch( $2.type)
		{
			case INTEGER:
				printf("%Ld\n", $2.lvalue);
				break;
			case FLOAT:
				printf("%Lg\n", $2.dvalue);
				break;
			case IDENTIFIER:
				printf( "%s\n", symbol_tables[ $2.index].buffer);		
				break;
		}
	}
	| lines ident '=' expr '\n'
	{
		symbol_tables[ $2.index].yylval = $4;
	}
	| lines '\n'
	| /* empty */
	| error '\n' { yyerror(" reenter previous line: "); yyerrok; }
	;
expr	: expr '+' expr	
	{
		switch( $1.type)
		{
			case INTEGER:	
				switch( $3.type)
				{
				case INTEGER:
					$$.type = INTEGER;
					$$.lvalue = $1.lvalue + $3.lvalue;
					break;
				case FLOAT:
					$$.type = FLOAT;
					$$.dvalue = (long double) $1.lvalue + $3.dvalue;
					break;
				}
				break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue + (long double) $3.lvalue;
						break;
					case FLOAT:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue + $3.dvalue;
						break;
				}
				break;
		}
	}
	| expr '-' expr
	{
		switch( $1.type)
		{
			case INTEGER:	
				switch( $3.type)
				{
				case INTEGER:
					$$.type = INTEGER;
					$$.lvalue = $1.lvalue - $3.lvalue;
					break;
				case FLOAT:
					$$.type = FLOAT;
					$$.dvalue = (long double) $1.lvalue - $3.dvalue;
					break;
				}
				break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue - (long double) $3.lvalue;
						break;
					case FLOAT:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue - $3.dvalue;
						break;
				}
				break;
		}
	}	
	| expr '*' expr	
	{
		switch( $1.type)
		{
			case INTEGER:	
				switch( $3.type)
				{
				case INTEGER:
					$$.type = INTEGER;
					$$.lvalue = $1.lvalue * $3.lvalue;
					break;
				case FLOAT:
					$$.type = FLOAT;
					$$.dvalue = (long double) $1.lvalue * $3.dvalue;
					break;
				}
				break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue * (long double) $3.lvalue;
						break;
					case FLOAT:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue * $3.dvalue;
						break;
				}
				break;
		}
	}
	| expr '/' expr	
	{
		switch( $1.type)
		{
			case INTEGER:	
				switch( $3.type)
				{
				case INTEGER:
					$$.type = INTEGER;
					$$.lvalue = $1.lvalue / $3.lvalue;
					break;
				case FLOAT:
					$$.type = FLOAT;
					$$.dvalue = (long double) $1.lvalue / $3.dvalue;
					break;
				}
				break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue / (long double) $3.lvalue;
						break;
					case FLOAT:
						$$.type = FLOAT;
						$$.dvalue = $1.dvalue / $3.dvalue;
						break;
				}
				break;
		}
	}

	| '(' expr ')'
	{
		switch( $2.type)
		{
			case INTEGER:
				$$.type = INTEGER;
				$$.lvalue = $2.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = $2.dvalue;
				break;
		}
	}
	| '-' expr %prec UMINUS 
	{
		switch( $2.type)
		{
			case INTEGER:
				$$.type = INTEGER;
				$$.lvalue = -1 * $2.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = -1 * (long double) $2.dvalue;
		}
	}
	| expr '|' expr
	{
		$$.type = INTEGER;
		switch( $1.type)
		{
			case INTEGER:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = $1.lvalue | $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue =  $1.lvalue | (long long int) $3.dvalue;
				}
			break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = (long long int) $1.dvalue | $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = (long long int) $1.dvalue | (long long int) $3.dvalue;
						break;
				}
			break;
		}
	}
	| expr '^' expr
	{
		$$.type = INTEGER;
		switch( $1.type)
		{
			case INTEGER:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = $1.lvalue ^ $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = $1.lvalue ^ (long long int) $3.dvalue;
				}
			break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = (long long int) $1.dvalue ^ $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = (long long int) $1.dvalue ^ (long long int) $3.dvalue;
						break;
				}
			break;
		}
	}
	| expr '&' expr	
	{
		$$.type = INTEGER;
		switch( $1.type)
		{
			case INTEGER:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = $1.lvalue & $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = $1.lvalue & (long long int) $3.dvalue;
				}
			break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = (long long int) $1.dvalue & $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = (long long int) $1.dvalue & (long long int) $3.dvalue;
						break;
				}
			break;
		}
	}
	| expr '%' expr
	{
		$$.type = INTEGER;
		switch( $1.type)
		{
			case INTEGER:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = $1.lvalue % $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = $1.lvalue % (long long int) $3.dvalue;
				}
			break;
			case FLOAT:
				switch( $3.type)
				{
					case INTEGER:
						$$.lvalue = (long long int) $1.dvalue % $3.lvalue;
						break;
					case FLOAT:
						$$.lvalue = (long long int) $1.dvalue % (long long int) $3.dvalue;
						break;
				}
			break;
		}
	}
	| '~' expr
	{
		$$.type = INTEGER;
		switch( $1.type)
		{
			case INTEGER:
				$$.lvalue = ~ $2.lvalue;
				break;	
			case FLOAT:
				$$.lvalue = ~ (long long int) $2.dvalue;
				break;
		}
	}
	| number 
	| ident
	{
		$$ = symbol_tables[ $1.index].yylval;
	}
	;

number	: INTEGER
	| FLOAT
	;

ident	: IDENTIFIER
	;
%%

void	yyerror( char *s)
{
	printf( "%s\n", s);
}

