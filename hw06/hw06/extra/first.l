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
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{
#include	"yystype.h"
#include	"y.tab.h"

%}

%%

"$"			{ return( EOF); }
[\n]			{ return( '\n'); /* return cr */ }
0[xX]{H}+{IS}?		{ sscanf( yytext, "%Lx", &yylval.lvalue); yylval.type = INTEGER; yylval.format = FORMAT_HEXADECIMAL; return( INTEGER); }
0{D}+{IS}?		{ sscanf( yytext, "%Lo", &yylval.lvalue); yylval.type = INTEGER; yylval.format = FORMAT_OCTAL; return( INTEGER); }
{D}+{IS}?		{ sscanf( yytext, "%Ld", &yylval.lvalue); yylval.type = INTEGER; yylval.format = FORMAT_DECIMAL; return( INTEGER); }
{D}+{E}{FS}?		{ sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
{D}*"."{D}+({E})?{FS}?	{ sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
{D}+"."{D}*({E})?{FS}?	{ sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
[ \t]			{ /* ignore white characters */ }
.			{ return( yytext[ 0]); /* return everything else */ }

%%

/*
 *	yy wrap function	always return NO
 */
yywrap()
{
	return( 1);
}

