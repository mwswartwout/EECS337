/*******************************************************************************
*
* FILE:		tokens.c
*
* DESC:		EECS 337 Assignment 2
*
* AUTHOR:	caseid
*
* DATE:		September 3, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
/*
 *	TOKENS
 */
#define IDENTIFIER 258
#define CONSTANT 259
#define STRING_LITERAL 260
#define SIZEOF 261
#define PTR_OP 262
#define INC_OP 263
#define DEC_OP 264
#define LEFT_OP 265
#define RIGHT_OP 266
#define LE_OP 267
#define GE_OP 268
#define EQ_OP 269
#define NE_OP 270
#define AND_OP 271
#define OR_OP 272
#define MUL_ASSIGN 273
#define DIV_ASSIGN 274
#define MOD_ASSIGN 275
#define ADD_ASSIGN 276
#define SUB_ASSIGN 277
#define LEFT_ASSIGN 278
#define RIGHT_ASSIGN 279
#define AND_ASSIGN 280
#define XOR_ASSIGN 281
#define OR_ASSIGN 282
#define TYPE_NAME 283
#define TYPEDEF 284
#define EXTERN 285
#define STATIC 286
#define AUTO 287
#define REGISTER 288
#define CHAR 289
#define SHORT 290
#define INT 291
#define LONG 292
#define SIGNED 293
#define UNSIGNED 294
#define FLOAT 295
#define DOUBLE 296
#define CONST 297
#define VOLATILE 298
#define VOID 299
#define STRUCT 300
#define UNION 301
#define ENUM 302
#define ELIPSIS 303
#define RANGE 304
#define CASE 305
#define DEFAULT 306
#define IF 307
#define ELSE 308
#define SWITCH 309
#define WHILE 310
#define DO 311
#define FOR 312
#define GOTO 313
#define CONTINUE 314
#define BREAK 315
#define RETURN 316
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
	if( token < 256)
		printf( "token: '%c' text: %s\n", token, text);
	else
		printf( "token: %s text: %s\n", tokens[ (token - IDENTIFIER) % sizeof( tokens)], text);
}

