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
{L}({L}|{D})*	{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_NONE); return( IDENTIFIER); }
0[xX]{H}+{IS}?	{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_HEXADECIMAL); return( CONSTANT);} 
0{D}+{IS}? 	{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_OCTAL): return( CONSTANT); }
{D}+{IS}? 	{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_DECIMAL); return( CONSTANT); }
{D}+{E}{FS}? 	{ yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_FLOAT): return( CONSTANT); }
{D}*"."{D}+({E})?{FS}? { yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_FLOAT): return( CONSTANT); } 
{D}+"."{D}*({E})?{FS}? { yylval.index = install( TYPE_IDENTIFIER, yytext, yyleng + 1, FORMAT_FLOAT): return( CONSTANT);}
" "		{/* Do nothing*/}
\n|.		{ return( yytext[ 0]); /* return everything else */ }


%%

int	install( int type, char *buffer, unsigned int length, int format)
{
	int index;
	
	for( index = 1; index <= data.index; index++)
		if( data.st[ index].type == type && data.st[ index].length == length && !strcmp( data.st[ index].buffer, buffer))
			return index;
		data.index++;
		data.st[ data.index].type = type;
		strcpy( data.st[ data.index].buffer, buffer);
		data.st[ data.index].length = length;
		data.st[ data.index].format = format;
		return data.index;
};
/*
 *	yy wrap function	always return NO
 */
yywrap()
{
	return( 1);
}

