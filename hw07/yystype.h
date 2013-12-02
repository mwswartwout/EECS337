/*******************************************************************************
*
* FILE:		yystype.h 
*
* DESC:		EECS 337 Assignment 6
*      		include file for the Calculator page 295-296
*
* AUTHOR:	caseid
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*		Updated for EECS 337 Assignment 7 October 15, 2013 
*
******************************************************************************/
#ifndef	YYSTYPE_H
#define	YYSTYPE_H	1

/*
 *	for debugging
 */
#define	YYDEBUG	1
/*
 *	define yystype
 */
#ifndef YYSTYPE
#define YYSTYPE struct yycalc
YYSTYPE
{
	int		index;	// index into static symbol table
	int		type;	// holds INTEGER or FLOAT from %token
#define	FORMAT_NONE		0
#define	FORMAT_DECIMAL		1
#define	FORMAT_HEXADECIMAL	2
#define	FORMAT_OCTAL		3
#define	FORMAT_FLOAT		4	// defined but not used
	int	format;
	long long	lvalue;	// holds long long value
	long double	dvalue;	// holds long double value
};
#endif


/*
 *	define a symbol table structure
 */
#define	SYMBOL_TABLE	struct symbol_table
SYMBOL_TABLE
{
#define	MAX_BUFFER_SIZE	128
	char	buffer[ MAX_BUFFER_SIZE];
	int	length;
	YYSTYPE	yylval;
};

/*
 *	lex variables
 */
extern SYMBOL_TABLE symbol_tables[];
extern unsigned int	symbol_table_index;
extern FILE	*yyin;
extern FILE	*yyout;
extern	char	*yytext;
extern	int	yydebug; 
extern	YYSTYPE yylval;
extern	int	yyparse();
extern  void	yyerror( char *);
#endif
