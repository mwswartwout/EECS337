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

[\n]	{ yylval.token = (int) yytext[0]; return yytext[ 0]; }
.	{ yylval.token = (int) yytext[0]; return yytext[ 0]; } /* return everything else */

%%

/*
 *	yywrap function
 */
int	yywrap( void)
{
	return(1);
}
