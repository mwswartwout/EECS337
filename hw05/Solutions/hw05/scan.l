/*******************************************************************************
*
* FILE:		scan.l
*
* DESC:		EECS 337 Assignment 5
* 		To avoid the syntax errors in test2.c we spell out the a, b, and
*		newline (\n) characters and filter away everything else.
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

/*
 *	extra credit
 */
void	count( void)
{
	static	int state = 0;

	if( state)
	{
	     data.message[ 0] = 0;
	     state = 0;
	     data.column = 0;
	}
	if( yytext[ 0] == '\n')
	{
	     state = 1;
	}
	else
	{
	     strcat( data.message, yytext);
	     if( yytext[ 0] == '\t')
	     	 data.column += 8 - (data.column % 8);
	     else
	     	 data.column++;
	}
	return;
}

%}

%%
"a"	{ count(); yylval = (int) yytext[0]; return yytext[ 0]; }
"b"	{ count(); yylval = (int) yytext[0]; return yytext[ 0]; }
[\n]	{ count(); yylval = (int) yytext[0]; return yytext[ 0]; }
.	{ count(); /* do nothing */ }

%%

/*
 *	yywrap function
 */
int	yywrap( void)
{
	return(1);
}

/*
 *	resync the parser with a syntax error
 */
void yysync( void)
{
	int c;
	data.errors++;
	while (( c = getc( yyin )) != EOF)
	{
		if( c == '\n')
		{
		    unput( c);
		    break;
		}
	}
}
