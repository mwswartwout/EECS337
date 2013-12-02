/*******************************************************************************
*
* FILE:		scan.l
*
* DESC:		EECS 337 Project 
*
* AUTHOR:	mws85
*
* DATE:		December 5, 2013
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
#include "yystype.h"
#include "y.tab.h"
#include "tokens.h"

%}

%%
"/*"			{ comment(); }
"//"			{ comment2(); }
"#"			{ comment2(); }

"auto"			{ count(); yylval.token = AUTO; return(AUTO); }
"break"			{ count(); yylval.token = BREAK; return(BREAK); }
"case"			{ count(); yylval.token = CASE; return(CASE); }
"char"			{ count(); yylval.token = CHAR; return(CHAR); }
"const"			{ count(); yylval.token = CONST: return(CONST); }
"continue"		{ count(); yylval.token = CONTINUE; return(CONTINUE); }
"default"		{ count(); yylval.token = DEFAULT; return(DEFAULT); }
"do"			{ count(); yylval.token = DO; return(DO); }
"double"		{ count(); yylval.token = DOUBLE; return(DOUBLE); }
"else"			{ count(); yylval.token = ELSE; return(ELSE); }
"enum"			{ count(); yylval.token = ENUM; return(ENUM); }
"extern"		{ count(); yylval.token = EXTERN; return(EXTERN; }
"float"			{ count(); yylval.token = FLOAT; return(FLOAT); }
"for"			{ count(); yylval.token = FOR; return(FOR); }
"goto"			{ count(); yylval.token = GOTO; return(GOTO); }
"if"			{ count(); yylval.token = IF; return(IF); }
"int"			{ count(); yylval.token = INT; return(INT); }
"long"			{ count(); yylval.token = LONG; return(LONG); }
"register"		{ count(); yylval.token = REGISTER; return(REGISTER); }
"return"		{ count(); yylval.token = RETURN; return(RETURN); }
"short"			{ count(); yylval.token = SHORT; return(SHORT); }
"signed"		{ count(); yylval.token = SIGNED; return(SIGNED); }
"sizeof"		{ count(); yylval.token = SIZEOF; return(SIZEOF); }
"static"		{ count(); yylval.token = STATIC; return(STATIC); }
"struct"		{ count(); yylval.token = STRUCT; return(STRUCT); }
"switch"		{ count(); yylval.token = SWITCH; return(SWITCH); }
"typedef"		{ count(); yylval.token = TYPEDEF; return(TYPEDEF); }
"union"			{ count(); yylval.token = UNION: return(UNION); }
"unsigned"		{ count(); yylval.token = UNSIGNED; return(UNSIGNED); }
"void"			{ count(); yylval.token = VOID; return(VOID); }
"volatile"		{ count(); yylval.token = VOLATILE; return(VOLATILE); }
"while"			{ count(); yylval.token = WHILE; return(WHILE); }

{L}({L}|{D})*		{ count(); yylval.tuple = identifier( yytext, yyleng); return(check_type());  }
0[xX]{H}+{IS}?		{ count(); yylval.tuple = constant_hex( yytext, yyleng); return(CONSTANT); }
0{D}+{IS}?		{ count(); yylval.tuple = constant_octal( yytext, yyleng); return(CONSTANT); }
{D}+{IS}?		{ count(); yylval.tuple = constant_decimal( yytext, yyleng); return(CONSTANT); }
'(\\.|[^\\'])+'		{ count(); yylval.tuple = consant_char( yytext, yyleng); return(CONSTANT); }
{D}+{E}{FS}?		{ count(); yylval.tuple = constant_float( yytext, yyleng); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(); yylval.tuple = constant_float( yytext, yyleng); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(); yylval.tuple = constant_float( yytext, yyleng); return(CONSTANT); }
\"(\\.|[^\\"])*\"	{ count(); yylval.tuple = constant_string_literal( yytext, yyleng); return(STRING_LITERAL); }

">>="			{ count(); yylval.token = RIGHT_ASSIGN; return(RIGHT_ASSIGN); }
"<<="			{ count(); yylval.token = LEFT_ASSIGN; return(LEFT_ASSIGN); }
"+="			{ count(); yylval.token = ADD_ASSIGN; return(ADD_ASSIGN); }
"-="			{ count(); yylval.token = SUB_ASSIGN; return(SUB_ASSIGN); }
"/="			{ count(); yylval.token = DIV_ASSIGN; return(DIV_ASSIGN); }
"%="			{ count(); yylval.token = MOD_ASSIGN; return(MOD_ASSIGN); }
"&="			{ count(); yylval.token = AND_ASSIGN; return(AND_ASSIGN); }
"^="			{ count(); yylval.token = XOR_ASSIGN; return(XOR_ASSIGN); }
"|="			{ count(); yylval.token = OR_ASSIGN; return(OR_ASSIGN); }
">>"			{ count(); yylval.token = RIGHT_OP; return(RIGHT_OP); }
"<<"			{ count(); yylval.token = LEFT_OP; return(LEFT_OP); }
"++"			{ count(); yylval.token = INC_OP; return(INC_OP); }
"--"			{ count(); yylval.token = DEC_OP; return(DEC_OP); }
"->"			{ count(); yylval.token = PRT_OP; return(PTR_OP); }
"&&"			{ count(); yylval.token = AND_OP; return(AND_OP); }
"||"			{ count(); yylval.token = OR_OP; return(OR_OP); }
"<="			{ count(); yylval.token = LE_OP; return(LE_OP); }
">="			{ count(); yylval.token = GE_OP; return(GE_OP); }
"=="			{ count(); yylval.token = EQ_OP; return(EQ_OP); }
"!="			{ count(); yylval.token = NE_OP; return(NE_OP); }
";"			{ count(); yylval.token = ';'; return(';'); }
"{"			{ count(); yylval.token = '{'; return('{'); }
"}"			{ count(); yylval.token = '}'; return('}'); }
","			{ count(); yylval.token = ','; return(','); }
":"			{ count(); yylval.token = ':'; return(':'); }
"="			{ count(); yylval.token = '='; return('='); }
"("			{ count(); yylval.token = '('; return('('); }
")"			{ count(); yylval.token = ')'; return(')'); }
"["			{ count(); yylval.token = '['; return('['); }
"]"			{ count(); yylval.token = ']'; return(']'); }
"."			{ count(); yylval.token = '.'; return('.'); }
"&"			{ count(); yylval.token = '&'; return('&'); }
"!"			{ count(); yylval.token = '!'; return('!'); }
"~"			{ count(); yylval.token = '~'; return('~'); }
"-"			{ count(); yylval.token = '-'; return('-'); }
"+"			{ count(); yylval.token = '+'; return('+'); }
"*"			{ count(); yylval.token = '*'; return('*'); }
"/"			{ count(); yylval.token = '/'; return('/'); }
"%"			{ count(); yylval.token = '%'; return('%'); }
"<"			{ count(); yylval.token = '<'; return('<'); }
">"			{ count(); yylval.token = '>'; return('>'); }
"^"			{ count(); yylval.token = '^'; return('^'); }
"|"			{ count(); yylval.token = '|'; return('|'); }
"?"			{ count(); yylval.token = '?'; return('?'); }

[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }

%%

/*
 *	yywrap function
 */
int	yywrap( void)
{
	return(1);
}

/*
 *	c preprocessor function
 */
void	pdefine( void)
{
	char c;

	if( IS_FLAGS_ECHO( data.flags))
	    ECHO;
	while(( c = input()) != '\n' && c != 0)
	      if( c && IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
	if( c == '\n')
		data.line++;
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
}

/*
 *	c comment function
 */
void	comment( void)
{
	char c, c1;

	if( IS_FLAGS_ECHO( data.flags))
	    ECHO;
loop:
	while(( c = input()) != '*' && c != 0)
	{
		if( c && IS_FLAGS_ECHO( data.flags))
			putchar(c);
		if( c == '\n')
			data.line++;
	}
	if( c == '\n')
		data.line++;
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);

	if(( c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}
	if( c == '\n')
		data.line++;
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c1);
}

/*
 *	c comment2 function (// EOL)
 */
void	comment2( void)
{
	char c;

	if( IS_FLAGS_ECHO( data.flags))
	    ECHO;
	while ((c = input()) != '\n' && c != 0)
	{
		if( c && IS_FLAGS_ECHO( data.flags))
			putchar(c);
	}
	if( c == '\n')
		data.line++;
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
}

/*
 *	update the current column index value
 */
void	count( void)
{
	int i;

	strcat( data.string, yytext);
	for( i = 0; yytext[i] != '\0'; i++)
		if( yytext[i] == '\n')
		{
			data.column = 0;
			data.line++;
			data.string[0] = 0;
		}
		else if( yytext[i] == '\t')
			data.column += 8 - (data.column % 8);
		else
			data.column++;
	if( yytext[0] && IS_FLAGS_ECHO( data.flags))
	    ECHO;
}

int check_type()
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/
	TUPLE	*tuple;
/*
 *	search the tuple list using the string compare to match
 */
	for( tuple = data.typedef_table; tuple; tuple = tuple->next)
	{
		if( ! strcmp( tuple->buffer, yytext))
		{
/*
 *	found typedef symbol so deallocate identifier memory and return this type
 */
			free_tuple( yylval.tuple);
			yylval.token = tuple->token;
			return( yylval.token);

		}
	}
/*
*	it actually will only return IDENTIFIER
*/

	return(IDENTIFIER);
}

