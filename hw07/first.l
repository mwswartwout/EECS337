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
*		Updated for EECS 337 Assignment 7 October 15, 2013 
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

/*
 *	define the scanner symbol table (static)
 */
#define	MAX_IDENTIFIERS	128
SYMBOL_TABLE symbol_tables[ MAX_IDENTIFIERS];
unsigned int	symbol_table_index = 0;
/*
 *	scanner symbol table register function
 *	return an index into the symbol table (static)
 */
int	identifier( char *buffer, unsigned int length)
{
	int	index;
/*
 *	find the same string and return the index
 */
	for( index = 1; index <= symbol_table_index; index++)
	     if( symbol_tables[ index].length == length && !strcmp( symbol_tables[ index].buffer, buffer))
	     	 return index;
/*
 *	else update to the next symbol table field
 */
	symbol_table_index++;
	strcpy( symbol_tables[ symbol_table_index].buffer, buffer);
	symbol_tables[ symbol_table_index].length = length;	// includes the null 
	memset((void*)&(symbol_tables[ symbol_table_index].yylval), 0, sizeof( YYSTYPE));
	symbol_tables[ symbol_table_index].yylval.index = symbol_table_index;
	symbol_tables[ symbol_table_index].yylval.type = IDENTIFIER;
	return symbol_table_index;
};

%}

%%

"$"			{ return( EOF); }
[\n]			{ return( '\n'); /* return cr */ }

{L}({L}|{D})*		{ yylval.type = IDENTIFIER; yylval.index = identifier( yytext, yyleng + 1); return( IDENTIFIER); }

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

