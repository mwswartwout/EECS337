/*******************************************************************************
*
* FILE:		lex.l (first.l)
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
*		Updated for EECS 337 Assignment 8 October 29, 2013 
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
#include	"symbol_table.c"

/*
 *	scanner symbol table register function
 *	return an index into the symbol table (static)
 */
int	install( int type, char *buffer, unsigned int length, int format)
{
	int	index;
/*
 *	find the same string and return the index
 */
	for( index = 1; index <= data.index; index++)
	     if( data.st[ index].type == type && data.st[ index].length == length && !strcmp( data.st[ index].buffer, buffer))
	     	 return index;
/*
 *	else update to the next symbol table field
 */
	data.index++;
	data.st[ data.index].type = type;
	strcpy( data.st[ data.index].buffer, buffer);
	data.st[ data.index].length = length;	// includes the null 
	data.st[ data.index].format = format;
	return data.index;
};

%}

%%

"$"			{ return( EOF); }
"if"			{ return IF; }
"else"			{ return ELSE; }
[\n]			{ return( '\n'); /* return cr */ }

{L}({L}|{D})*		{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_NONE); return( IDENTIFIER); }

0[xX]{H}+{IS}?		{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_HEXADECIMAL); return( CONSTANT); }
0{D}+{IS}?		{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_OCTAL); return( CONSTANT); }
{D}+{IS}?		{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_DECIMAL); return( CONSTANT); }
{D}+{E}{FS}?		{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_FLOAT); return( CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_FLOAT); return( CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ yylval.index = install( TYPE_CONSTANT, yytext, yyleng + 1, FORMAT_FLOAT); return( CONSTANT); }
"<="			{ return( LE); }
">="			{ return( GE); }
"=="			{ return( EQ); }
"!="			{ return( NE); }
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

