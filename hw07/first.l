/*******************************************************************************
*
* FILE:		first.l
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

#define MAX_IDENTIFIERS 128
SYMBOL_TABLE symbol_tables[ MAX_IDENTIFIERS];
unsigned int symbol_table_index = 0;

int	identifier( char *buffer, unsigned int length)
{
	int i;
	for ( i = 0; i < MAX_IDENTIFIERS; i++)
	{
		if( !strncmp( buffer, symbol_tables[i].buffer, length))
			return i;
	}

	symbol_table_index++;
	symbol_tables[symbol_table_index].length = length;
	strcpy( symbol_tables[symbol_table_index].buffer, buffer);
	symbol_tables[symbol_table_index].yylval.type = IDENTIFIER;
	symbol_tables[symbol_table_index].yylval.index = symbol_table_index;
	return symbol_table_index;
}
%}

%%

"$"		{ return( EOF); }
0[xX]{H}+{IS}?	{ sscanf( yytext, "%Lx", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
0{D}+{IS}? 	{ sscanf( yytext, "%Lo", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
{D}+{IS}? 	{sscanf( yytext, "%Ld", &yylval.lvalue); yylval.type = INTEGER; return( INTEGER); }
{D}+{E}{FS}? 	{sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
{D}*"."{D}+({E})?{FS}? { sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); } 
{D}+"."{D}*({E})?{FS}? {sscanf( yytext, "%Lf", &yylval.dvalue); yylval.type = FLOAT; return( FLOAT); }
{L}({L}|{D})*	{ yylval.type = IDENTIFIER; yylval.index = identifier( yytext, yyleng + 1); return( IDENTIFIER); }
\n|.		{ return( yytext[ 0]); /* return everything else */ }


%%

/*
 *	yy wrap function	always return NO
 */
yywrap()
{
	return( 1);
}

