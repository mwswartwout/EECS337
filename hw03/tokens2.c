/*******************************************************************************
*
* FILE:		tokens2.c
*
* DESC:		EECS 337 Assignment 3
*
* AUTHOR:	caseid
*
* DATE:		September 10, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
/*
 *	string array of token names
 */
char	*tokens[] =
{
	"IDENTIFIER",
	"CONSTANT",
	"STRING_LITERAL",
	"SIZEOF",
	"PTR_OP",
	"INC_OP",
	"DEC_OP",
	"LEFT_OP",
	"RIGHT_OP",
	"LE_OP",
	"GE_OP",
	"EQ_OP",
	"NE_OP",
	"AND_OP",
	"OR_OP",
	"MUL_ASSIGN",
	"DIV_ASSIGN",
	"MOD_ASSIGN",
	"ADD_ASSIGN",
	"SUB_ASSIGN",
	"LEFT_ASSIGN",
	"RIGHT_ASSIGN",
	"AND_ASSIGN",
	"XOR_ASSIGN",
	"OR_ASSIGN",
	"TYPE_NAME",
	"TYPEDEF",
	"EXTERN",
	"STATIC",
	"AUTO",
	"REGISTER",
	"CHAR",
	"SHORT",
	"INT",
	"LONG",
	"SIGNED",
	"UNSIGNED",
	"FLOAT",
	"DOUBLE",
	"CONST",
	"VOLATILE",
	"VOID",
	"STRUCT",
	"UNION",
	"ENUM",
	"ELIPSIS",
	"RANGE",
	"CASE",
	"DEFAULT",
	"IF",
	"ELSE",
	"SWITCH",
	"WHILE",
	"DO",
	"FOR",
	"GOTO",
	"CONTINUE",
	"BREAK",
	"RETURN",
	"",
};
/*
 *	print the token and text string
 */
void	print_token( int token, char *text)
{
	switch( token)
	{
	case ' ':
	case '\t':
	case '\v':
	case '\n':
	case '\f':
		break;	/* skip white space characters */
	default:
		if( token < 256)
			printf( "token: '%c' text: %s\n", token, text);
		else
			printf( "token: %s text: %s\n", tokens[ (token - IDENTIFIER) % sizeof( tokens)], text);
	}
}

