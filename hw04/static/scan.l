/*******************************************************************************
*
* FILE:		scan.l
*
* DESC:		EECS 337 Assignment 3
*
* AUTHOR:	caseid
*
* DATE:		September 10, 2013
*
* EDIT HISTORY:	
*
*	Updated for Assignment 4 September 17, 2013
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
"//"			{ comment2(); }
"#"			{ comment2(); }

"auto"			{ count(AUTO); yylval.token = AUTO; return(AUTO); }
"break"			{ count(BREAK); yylval.token = BREAK; return(BREAK); }
"case"			{ count(CASE); yylval.token = CASE; return(CASE); }
"char"			{ count(CHAR); yylval.token = CHAR; return(CHAR); }
"const"			{ count(CONST); yylval.token = CONST; return(CONST); }
"continue"		{ count(CONTINUE); yylval.token = CONTINUE; return(CONTINUE); }
"default"		{ count(DEFAULT); yylval.token = DEFAULT; return(DEFAULT); }
"do"			{ count(DO); yylval.token = DO; return(DO); }
"double"		{ count(DOUBLE); yylval.token = DOUBLE; return(DOUBLE); }
"else"			{ count(ELSE); yylval.token = ELSE; return(ELSE); }
"enum"			{ count(ENUM); yylval.token = ENUM; return(ENUM); }
"extern"		{ count(EXTERN); yylval.token = EXTERN; return(EXTERN); }
"float"			{ count(FLOAT); yylval.token = FLOAT; return(FLOAT); }
"for"			{ count(FOR); yylval.token = FOR; return(FOR); }
"goto"			{ count(GOTO); yylval.token = GOTO; return(GOTO); }
"if"			{ count(IF); yylval.token = IF; return(IF); }
"int"			{ count(INT); yylval.token = INT; return(INT); }
"long"			{ count(LONG); yylval.token = LONG; return(LONG); }
"register"		{ count(REGISTER); yylval.token = REGISTER; return(REGISTER); }
"return"		{ count(RETURN); yylval.token = RETURN; return(RETURN); }
"short"			{ count(SHORT); yylval.token = SHORT; return(SHORT); }
"signed"		{ count(SIGNED); yylval.token = SIGNED; return(SIGNED); }
"sizeof"		{ count(SIZEOF); yylval.token = SIZEOF; return(SIZEOF); }
"static"		{ count(STATIC); yylval.token = STATIC; return(STATIC); }
"struct"		{ count(STRUCT); yylval.token = STRUCT; return(STRUCT); }
"switch"		{ count(SWITCH); yylval.token = SWITCH; return(SWITCH); }
"typedef"		{ count(TYPEDEF); yylval.token = TYPEDEF; return(TYPEDEF); }
"union"			{ count(UNION); yylval.token = UNION; return(UNION); }
"unsigned"		{ count(UNSIGNED); yylval.token = UNSIGNED; return(UNSIGNED); }
"void"			{ count(VOID); yylval.token = VOID; return(VOID); }
"volatile"		{ count(VOLATILE); yylval.token = VOLATILE; return(VOLATILE); }
"while"			{ count(WHILE); yylval.token = WHILE; return(WHILE); }

{L}({L}|{D})*		{ count(IDENTIFIER); yylval.token = IDENTIFIER; yylval.index = attribute( IDENTIFIER, yytext, yyleng, FORMAT_NONE); return(check_type()); }

0[xX]{H}+{IS}?		{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_HEXADECIMAL); return(CONSTANT); }
0{D}+{IS}?		{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_OCTAL); return(CONSTANT); }
{D}+{IS}?		{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_DECIMAL); return(CONSTANT); }
'(\\.|[^\\'])+'		{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_CHAR); return(CONSTANT); }

{D}+{E}{FS}?		{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_FLOAT); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_FLOAT); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ count(CONSTANT); yylval.token = CONSTANT; yylval.index = attribute( CONSTANT, yytext, yyleng, FORMAT_FLOAT); return(CONSTANT); }

\"(\\.|[^\\"])*\"	{ count(STRING_LITERAL); yylval.token = STRING_LITERAL; yylval.index = attribute( STRING_LITERAL, yytext, yyleng, FORMAT_NONE); return(STRING_LITERAL); }

">>="			{ count(RIGHT_ASSIGN); yylval.token = RIGHT_ASSIGN; return(RIGHT_ASSIGN); }
"<<="			{ count(LEFT_ASSIGN); yylval.token = LEFT_ASSIGN; return(LEFT_ASSIGN); }
"+="			{ count(ADD_ASSIGN); yylval.token = ADD_ASSIGN; return(ADD_ASSIGN); }
"-="			{ count(SUB_ASSIGN); yylval.token = SUB_ASSIGN; return(SUB_ASSIGN); }
"*="			{ count(MUL_ASSIGN); yylval.token = MUL_ASSIGN; return(MUL_ASSIGN); }
"/="			{ count(DIV_ASSIGN); yylval.token = DIV_ASSIGN; return(DIV_ASSIGN); }
"%="			{ count(MOD_ASSIGN); yylval.token = MOD_ASSIGN; return(MOD_ASSIGN); }
"&="			{ count(AND_ASSIGN); yylval.token = AND_ASSIGN; return(AND_ASSIGN); }
"^="			{ count(XOR_ASSIGN); yylval.token = XOR_ASSIGN; return(XOR_ASSIGN); }
"|="			{ count(OR_ASSIGN); yylval.token = OR_ASSIGN; return(OR_ASSIGN); }
">>"			{ count(RIGHT_OP); yylval.token = RIGHT_OP; return(RIGHT_OP); }
"<<"			{ count(LEFT_OP); yylval.token = LEFT_OP; return(LEFT_OP); }
"++"			{ count(INC_OP); yylval.token = INC_OP; return(INC_OP); }
"--"			{ count(DEC_OP); yylval.token = DEC_OP; return(DEC_OP); }
"->"			{ count(PTR_OP); yylval.token = PTR_OP; return(PTR_OP); }
"&&"			{ count(AND_OP); yylval.token = AND_OP; return(AND_OP); }
"||"			{ count(OR_OP); yylval.token = OR_OP; return(OR_OP); }
"<="			{ count(LE_OP); yylval.token = LE_OP; return(LE_OP); }
">="			{ count(GE_OP); yylval.token = GE_OP; return(GE_OP); }
"=="			{ count(EQ_OP); yylval.token = EQ_OP; return(EQ_OP); }
"!="			{ count(NE_OP); yylval.token = NE_OP; return(NE_OP); }
";"			{ count(';'); yylval.token = ';'; return(';'); }
"{"			{ count('{'); yylval.token = '{'; return('{'); }
"}"			{ count('}'); yylval.token = '}'; return('}'); }
","			{ count(','); yylval.token = ','; return(','); }
":"			{ count(':'); yylval.token = ':'; return(':'); }
"="			{ count('='); yylval.token = '='; return('='); }
"("			{ count('('); yylval.token = '('; return('('); }
")"			{ count(')'); yylval.token = ')'; return(')'); }
"["			{ count('['); yylval.token = '['; return('['); }
"]"			{ count(']'); yylval.token = ']'; return(']'); }
"."			{ count('.'); yylval.token = '.'; return('.'); }
"&"			{ count('&'); yylval.token = '&'; return('&'); }
"!"			{ count('!'); yylval.token = '!'; return('!'); }
"~"			{ count('~'); yylval.token = '~'; return('~'); }
"-"			{ count('-'); yylval.token = '-'; return('-'); }
"+"			{ count('+'); yylval.token = '+'; return('+'); }
"*"			{ count('*'); yylval.token = '*'; return('*'); }
"/"			{ count('/'); yylval.token = '/'; return('/'); }
"%"			{ count('%'); yylval.token = '%'; return('%'); }
"<"			{ count('<'); yylval.token = '<'; return('<'); }
">"			{ count('>'); yylval.token = '>'; return('>'); }
"^"			{ count('^'); yylval.token = '^'; return('^'); }
"|"			{ count('|'); yylval.token = '|'; return('|'); }
"?"			{ count('?'); yylval.token = '?'; return('?'); }

[ \t\v\n\f]		{ count( yytext[0]); /* skip white space characters */ }
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

/*
 *	c slash-slash comment function
 */
void	comment2( void)
{
	char c;

	if( IS_FLAGS_ECHO( data.flags))
	    ECHO;
	while(( c = input()) != '\n' && c != 0)
	      if( c && IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
	if( c != 0)
	      if( IS_FLAGS_ECHO( data.flags))
	      	  putchar(c);
}

void count( int token)
{
	int i;

	if( IS_FLAGS_DEBUG( data.flags))
	    print_token( token, yytext);

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
