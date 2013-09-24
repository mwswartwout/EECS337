/*******************************************************************************
*
* FILE:		scan.l
*
* DESC:		EECS 337 Assignment 3
*
* AUTHOR:	mws85
*
* DATE:		September 23, 2013
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
#include	"tokens2.c"

%}

%%
"/*"			{ comment(); }
"//"			{ commentdouble(); }
"#"			{ commentdouble(); }
"auto"			{ count(AUTO); return(AUTO); }
"break"			{ count(BREAK); return(BREAK); }
"case"			{ count(CASE); return(CASE); }
"char"			{ count(CHAR); return(CHAR); }
"const"			{ count(CONST); return(CONST); }
"continue"		{ count(CONTINUE); return(CONTINUE); }
"default"		{ count(DEFAULT); return(DEFAULT); }
"do"			{ count(DO); return(DO); }
"double"		{ count(DOUBLE); return(DOUBLE); }
"else"			{ count(ELSE); return(ELSE); }
"enum"			{ count(ENUM); return(ENUM); }
"extern"		{ count(EXTERN); return(EXTERN); }
"float"			{ count(FLOAT); return(FLOAT); }
"for"			{ count(FOR); return(FOR); }
"goto"			{ count(GOTO); return(GOTO); }
"if"			{ count(IF); return(IF); }
"int"			{ count(INT); return(INT); }
"long"			{ count(LONG); return(LONG); }
"register"		{ count(REGISTER); return(REGISTER); }
"return"		{ count(RETURN); return(RETURN); }
"short"			{ count(SHORT); return(SHORT); }
"signed"		{ count(SIGNED); return(SIGNED); }
"sizeof"		{ count(SIZEOF); return(SIZEOF); }
"static"		{ count(STATIC); return(STATIC); }
"struct"		{ count(STRUCT); return(STRUCT); }
"switch"		{ count(SWITCH); return(SWITCH); }
"typedef"		{ count(TYPEDEF); return(TYPEDEF); }
"union"			{ count(UNION); return(UNION); }
"unsigned"		{ count(UNSIGNED); return(UNSIGNED); }
"void"			{ count(VOID); return(VOID); }
"volatile"		{ count(VOLATILE); return(VOLATILE); }
"while"			{ count(WHILE); return(WHILE); }

{L}({L}|{D})*		{ count(check_type()); return(check_type()); }

0[xX]{H}+{IS}?		{ count(CONSTANT); return(CONSTANT); }
0{D}+{IS}?		{ count(CONSTANT); return(CONSTANT); }
{D}+{IS}?		{ count(CONSTANT); return(CONSTANT); }
'(\\.|[^\\'])+'		{ count(CONSTANT); return(CONSTANT); }

{D}+{E}{FS}?		{ count(CONSTANT); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(CONSTANT); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(CONSTANT); return(CONSTANT); }

\"(\\.|[^\\"])*\"	{ count(STRING_LITERAL); return(STRING_LITERAL); }

">>="			{ count(RIGHT_ASSIGN); return(RIGHT_ASSIGN); }
"<<="			{ count(LEFT_ASSIGN); return(LEFT_ASSIGN); }
"+="			{ count(ADD_ASSIGN); return(ADD_ASSIGN); }
"-="			{ count(SUB_ASSIGN); return(SUB_ASSIGN); }
"*="			{ count(MUL_ASSIGN); return(MUL_ASSIGN); }
"/="			{ count(DIV_ASSIGN); return(DIV_ASSIGN); }
"%="			{ count(MOD_ASSIGN); return(MOD_ASSIGN); }
"&="			{ count(AND_ASSIGN); return(AND_ASSIGN); }
"^="			{ count(XOR_ASSIGN); return(XOR_ASSIGN); }
"|="			{ count(OR_ASSIGN); return(OR_ASSIGN); }
">>"			{ count(RIGHT_OP); return(RIGHT_OP); }
"<<"			{ count(LEFT_OP); return(LEFT_OP); }
"++"			{ count(INC_OP); return(INC_OP); }
"--"			{ count(DEC_OP); return(DEC_OP); }
"->"			{ count(PTR_OP); return(PTR_OP); }
"&&"			{ count(AND_OP); return(AND_OP); }
"||"			{ count(OR_OP); return(OR_OP); }
"<="			{ count(LE_OP); return(LE_OP); }
">="			{ count(GE_OP); return(GE_OP); }
"=="			{ count(EQ_OP); return(EQ_OP); }
"!="			{ count(NE_OP); return(NE_OP); }
";"			{ count(';' ); return(';'); }
"{"			{ count('{' ); return('{'); }
"}"			{ count('}' ); return('}'); }
","			{ count(',' ); return(','); }
":"			{ count(':' ); return(':'); }
"="			{ count('=' ); return('='); }
"("			{ count('(' ); return('('); }
")"			{ count(')'); return(')'); }
"["			{ count('['); return('['); }
"]"			{ count(']'); return(']'); }
"."			{ count('-'); return('.'); }
"&"			{ count('&'); return('&'); }
"!"			{ count('!'); return('!'); }
"~"			{ count('~'); return('~'); }
"-"			{ count('-'); return('-'); }
"+"			{ count('+'); return('+'); }
"*"			{ count('*'); return('*'); }
"/"			{ count('/'); return('/'); }
"%"			{ count('%'); return('%'); }
"<"			{ count('<'); return('<'); }
">"			{ count('>'); return('>'); }
"^"			{ count('^'); return('^'); }
"|"			{ count('|'); return('|'); }
"?"			{ count('?'); return('?'); }

[ \t\v\n\f]		{ count(yytext[ 0]); /* skip white space characters */ }
.			{ /* ignore bad characters */ }

%%

int yywrap( void)
{
	return(1);
}

void comment( void)
{
	char c, c1;

	if( IS_FLAGS_ECHO( data.flags))
	    ECHO;
loop:
	while(( c = input()) != '*' && c != 0)
	      if( c && IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);

	if(( c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c1);
}

void commentdouble( void)
{
	char c;
	if ( IS_FLAGS_ECHO( data.flags))
		ECHO;
	while(( c = input()) != '\n' && c != 0)
		if( c && IS_FLAGS_ECHO( data.flags))
			putchar( c);
		if ( c != 0)
			if( IS_FLAGS_ECHO( data.flags))
				putchar( c);
}

void count( int token)
{
	if( IS_FLAGS_DEBUG( data.flags))
		print_token( token, yytext);

	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			data.column = 0;
		else if (yytext[i] == '\t')
			data.column += 8 - (data.column % 8);
		else
			data.column++;
	if( yytext[0] && IS_FLAGS_ECHO( data.flags))
	    ECHO;
}

int check_type( void)
{
/*
 * pseudo code --- this is what it should check
 *
 *	if (yytext == type_name)
 *		return(TYPE_NAME);
 *
 *	return(IDENTIFIER);
 */
/*
 *	it actually will only return IDENTIFIER
 */
	return(IDENTIFIER);
}
