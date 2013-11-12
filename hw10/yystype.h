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
*		Updated for EECS 337 Assignment 7 October 15, 2013 
*		Updated for EECS 337 Assignment 8 October 29, 2013 
*		Updated for EECS 337 Assignment 9 November 5, 2013 
*
******************************************************************************/
#ifndef	YYSTYPE_H
#define	YYSTYPE_H	1

/*
 *	for debugging
 */
#define	YYDEBUG	1
/*
 *	symbol table
 */
#define	SYMBOL_TABLE	struct symbol_table
SYMBOL_TABLE
{
#define	MAX_BUFFER_SIZE	128
	char	buffer[ MAX_BUFFER_SIZE];
	int	length;
#define	TYPE_NONE		0
#define	TYPE_IDENTIFIER		1
#define	TYPE_CONSTANT		2
#define	TYPE_TEMPORARY		3
#define	TYPE_LABEL		4
	int	type;
#define	FORMAT_NONE		0
#define	FORMAT_DECIMAL		1
#define	FORMAT_HEXADECIMAL	2
#define	FORMAT_OCTAL		3
#define	FORMAT_FLOAT		4	// defined but not used
	int	format;
#define SPECIFIER_CHAR		1
#define	SPECIFIER_SHORT		2
#define SPECIFIER_INT		4
#define SPECIFIER_LONG		8
	int	specifier;
	int 	sizeofspecifier;
	int	sizeofarray;
};
/*
 *	define a QUAD data structure that supports:
 *	1) X = Y op Z 	   	     $$.quad = new_quad1( '+', $1.quad, $3.quad);
 *	2) X = op Y   	   	     $$.quad = new_quad2( UMINUS, $2.quad);
 *	3) X = Y      	   	     $$.quad = new_quad3( '=', $1.index, $3.quad);
 *	4) GOTO label
 *	5) IFTRUE X GOTO label
 *	5) IFFALSE X GOTO label
 *	6) IF X relop Y GOTO label
 *	7) CALL label,n
 *	7) Y = CALL label,n
 *	7) RETURN
 *	7) RETURN Y
 *	8) X = Y[ Z]
 *	8) X[ Y] = Z
 *	9) X = &Y
 *	9) X = *Y
 *	9) *X = Y
 */
#define	QUAD struct quad	
QUAD
{
	int	operator;
	int	dst_type;
	int	dst_index;
	int	op1_type;
	int	op1_index;
	int	op2_type;
	int	op2_index;
	QUAD	*next;
};
/*
 *	define yystype
 */
#ifndef YYSTYPE
#define YYSTYPE struct yycalc
YYSTYPE
{
	int		index;	// index into static symbol table
	QUAD		*quad;
};
#endif

/*
 *	define a global data structure
 */
#define	DATA	struct data
DATA
{
	int	flags;
#define	FLAGS_DEBUG	0x0001
#define	FLAGS_PARSE	0x0002
#define	FLAGS_SYMBOL	0x0004
#define	IS_FLAGS_DEBUG(a)	(a & FLAGS_DEBUG)	
#define	SET_FLAGS_DEBUG(a)	(a |= FLAGS_DEBUG)
#define	CLR_FLAGS_DEBUG(a)	(a &= ~FLAGS_DEBUG)
#define	IS_FLAGS_PARSE(a)	(a & FLAGS_PARSE)	
#define	SET_FLAGS_PARSE(a)	(a |= FLAGS_PARSE)
#define	CLR_FLAGS_PARSE(a)	(a &= ~FLAGS_PARSE)
#define	IS_FLAGS_SYMBOL(a)	(a & FLAGS_SYMBOL)	
#define	SET_FLAGS_SYMBOL(a)	(a |= FLAGS_SYMBOL)
#define	CLR_FLAGS_SYMBOL(a)	(a &= ~FLAGS_SYMBOL)
	int	memory;
	int	temp;
	int	label;
	int	errors;
/*
 *	define the symbol table 
 */
#define	MAX_SYMBOL_TABLES	128
	SYMBOL_TABLE st[ MAX_SYMBOL_TABLES];
	unsigned int	index;
};

/*
 *	external functions from quad.c
 */
extern	void	new_error( int length);
extern	QUAD	*new_quad( int operator, int dst_type, int dst_index, int op1_type, int op1_index, int op2_type, int op2_index);
extern	void	free_quad( QUAD *quad);
extern	void	free_quad_list( QUAD *quad_list);
extern	QUAD	*end_quad_list( QUAD *quad_list);
extern	void	print_quad_operand( int type, int index);
extern	void	print_quad( QUAD *quad);
extern	void	print_quad_list( QUAD *quad_list);
extern	int	next_temp( void);
extern	int	next_label( void);
extern	QUAD	*new_quad1( int operator, QUAD *q1, QUAD *q2);
extern	QUAD	*new_quad2( int operator, QUAD *q1);
extern	QUAD	*new_quad3( int operator, int index, QUAD *q1);
extern	QUAD	*new_quad5( int operator, QUAD *q1, QUAD *q2, QUAD *q3);
extern 	QUAD	*new_quad8( int operator, int index, QUAD *q1, QUAD *q2);
extern 	int 	new_symbol( int specifier, int identifier, int constant);
/*
 *	extern data from main.c
 */
extern DATA	data;
/*
 *	extern from symbol_table.c
 */
extern void	print_symbol_table( void);
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
