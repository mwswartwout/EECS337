/*******************************************************************************
*
* FILE:		yystype.h 
*
* DESC:		EECS 337 Assignment 6
*      		include file for the Calculator page 295-296
*
* AUTHOR:	mws85
*
* DATE:		October 8, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#ifndef	YYSTYPE_H
#define	YYSTYPE_H	1

/*
 *	for debugging
 */
#define	YYDEBUG	1
/*
 *	define yystype
 */
#define YYSTYPE struct yycalc
YYSTYPE
{
	int	type;	// holds INTEGER or FLOAT from %token
	long long	lvalue; 	//holds INTEGER value
	long double	dvalue;		// holds FLOAT value
};

#define SYMBOL_TABLE struct symbol_table
SYMBOL_TABLE
{
#define MAX_BUFFER_SIZE	128
	char	buffer[ MAX_BUFFER_SIZE];
	int	length;
	YYSTYPE yylval;
};

extern SYMBOL_TABLE symbol_tables[];
extern unsigned int symbol_table_index;


/*
 *	lex variables
 */
extern FILE	*yyin;
extern FILE	*yyout;
extern	char	*yytext;
extern	int	yydebug; 
extern	YYSTYPE yylval;
extern	int	yyparse();
extern  void	yyerror( char *);
#endif
