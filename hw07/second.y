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
*		Updated for EECS 337 Assignment 7 October 15, 2013 
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
		     switch( $2.format)
		     {
		     case FORMAT_DECIMAL:
			printf("%Ld\n", $2.lvalue);
			break;
		     case FORMAT_HEXADECIMAL:
			printf("0x%Lx\n", $2.lvalue);
			break;
		     case FORMAT_OCTAL:
			printf("0%Lo\n", $2.lvalue);
			break;
		     }
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
	| lines error '\n' { yyerror(" reenter previous line: "); yyerrok; }
	| /* empty */
	;
expr
	: expr '+' expr
	{ 
		switch( $1.type)
		{
		case INTEGER:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue + $3.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = (long double)$1.lvalue + $3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = FLOAT;
				$$.dvalue = $1.dvalue + (long double)$3.lvalue;
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
				$$.format = $1.format;
				$$.lvalue = $1.lvalue - $3.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = (long double)$1.lvalue - $3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = FLOAT;
				$$.dvalue = $1.dvalue - (long double)$3.lvalue;
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
				$$.format = $1.format;
				$$.lvalue = $1.lvalue * $3.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = (long double)$1.lvalue * $3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = FLOAT;
				$$.dvalue = $1.dvalue * (long double)$3.lvalue;
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
				$$.format = $1.format;
				$$.lvalue = $1.lvalue / $3.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = (long double)$1.lvalue / $3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = FLOAT;
				$$.dvalue = $1.dvalue / (long double)$3.lvalue;
				break;
			case FLOAT:
				$$.type = FLOAT;
				$$.dvalue = $1.dvalue / $3.dvalue;
				break;
			}
			break;
		}
	}
/* 	add bitwise operators */
	| expr '|' expr	
	{
		switch( $1.type)
		{
		case INTEGER:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue | $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue | (long long)$3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $3.format;
				$$.lvalue = (long long)$1.dvalue | $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = FORMAT_DECIMAL;
				$$.lvalue = (long long)$1.dvalue | (long long)$3.dvalue;
				break;
			}
			break;
		}
	}
	| expr '^' expr	
	{
		switch( $1.type)
		{
		case INTEGER:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue ^ $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue ^ (long long)$3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $3.format;
				$$.lvalue = (long long)$1.dvalue ^ $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = FORMAT_DECIMAL;
				$$.lvalue = (long long)$1.dvalue ^ (long long)$3.dvalue;
				break;
			}
			break;
		}
	}
	| expr '&' expr	
	{
		switch( $1.type)
		{
		case INTEGER:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue & $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue & (long long)$3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $3.format;
				$$.lvalue = (long long)$1.dvalue & $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = FORMAT_DECIMAL;
				$$.lvalue = (long long)$1.dvalue & (long long)$3.dvalue;
				break;
			}
			break;
		}
	}
	| expr '%' expr	
	{
		switch( $1.type)
		{
		case INTEGER:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue % $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = $1.format;
				$$.lvalue = $1.lvalue % (long long)$3.dvalue;
				break;
			}
			break;
		case FLOAT:
			switch( $3.type)
			{
			case INTEGER:
				$$.type = INTEGER;
				$$.format = $3.format;
				$$.lvalue = (long long)$1.dvalue % $3.lvalue;
				break;
			case FLOAT:
				$$.type = INTEGER;
				$$.format = FORMAT_DECIMAL;
				$$.lvalue = (long long)$1.dvalue % (long long)$3.dvalue;
				break;
			}
			break;
		}
	}
	| '(' expr ')'	
	{
		$$ = $2;
	}
	| '~' expr 
	{
		switch( $2.type)
		{
		case INTEGER:
			$$.type = INTEGER;
			$$.format = $2.format;
			$$.lvalue = ~ $2.lvalue;
			break;
		case FLOAT:
			$$.type = INTEGER;
			$$.format = FORMAT_DECIMAL;
			$$.lvalue = ~ (long long)$2.dvalue;
			break;
		}
	}
	| '-' expr %prec UMINUS 
	{
		switch( $2.type)
		{
		case INTEGER:
			$$.type = INTEGER;
			$$.format = $2.format;
			$$.lvalue = - $2.lvalue;
			break;
		case FLOAT:
			$$.type = FLOAT;
			$$.dvalue = - $2.dvalue;
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

