/*******************************************************************************
*
* FILE:		scan.l
*
* DESC:		EECS 337 Assignment 5
* 		To avoid the syntax errors in test2.c we spell out the a, b, and
*		newline (\n) characters and filter away everything else.
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

%%

[\n]	{ yylval = (int) yytext[0]; return yytext[ 0]; }
a	{ yylval = (int) yytext[0]; return yytext[ 0]; }
b	{ yylval = (int) yytext[0]; return yytext[ 0]; }
.	{  } /* everything else return's nothing */

%%

/*
*	resync the parser with a syntax error
*/
void yysync( void)
{
	int c;
	data.errors++;
	while (( c = getc( yyin)) != EOF)
	{
		if( c == '\n')
		{
			unput( c);
			break;
		}
	}
}

/*
 *	yywrap function
 */
int	yywrap( void)
{
	return(1);
}
