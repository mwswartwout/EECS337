/*******************************************************************************
*
* FILE:		lex.l
*
* DESC:		EECS 337 Assignment 6
*      		Scanner program for the Calculator page 296-296
*
* AUTHOR:	mws85
*
* DATE:		October 15, 2013
*
*
* EDIT HISTORY:	
*
*******************************************************************************/
D	[0-9]
L	[a-zA-Z_]
H	[a-fA-F0-9]
E	[Ee][+-]?{D}+
FS	(f|F|l|L)
IS	(u|U|l|L)*

%{
#include	"yystype.h"
#include	"y.tab.h"

#include 	"symbol_table.c"

%}

%%

"$"		{ return( EOF); }
{L}({L}|{D})*	{ yylval.type = IDENTIFIER; yylval.index = identifier( yytext, yyleng + 1); return( IDENTIFIER); }
0[xX]{H}+{IS}?	{ sscanf( yytext, "%Lx", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
0{D}+{IS}? 	{ sscanf( yytext, "%Lo", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
{D}+{IS}? 	{sscanf( yytext, "%Ld", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
{D}+{E}{FS}? 	{sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
{D}*"."{D}+({E})?{FS}? { sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); } 
{D}+"."{D}*({E})?{FS}? {sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
" "		{/* Do nothing*/}
\n|.		{ return( yytext[ 0]); /* return everything else */ }


%%

/*
 *	yy wrap function	always return NO
 */
yywrap()
{
	return( 1);
}

