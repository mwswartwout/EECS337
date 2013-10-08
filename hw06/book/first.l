/*******************************************************************************
*
* FILE:		first.l
*
* DESC:		EECS 337 Assignment 6
*      		Scanner program for the Calculator page 296-296
*
* AUTHOR:	caseid
*
* DATE:		October 8, 2013
*
*
* EDIT HISTORY:	
*
*******************************************************************************/
number	[0-9]+\e.?|[0-9]*\e.[0-9]+

%%

{number}	{ sscanf( yytext, "%lf", &yylval); return NUMBER; }
\n|.		{ return( yytext[ 0]); /* return everything else */ }

