/*******************************************************************************
*
* FILE:		yystype.h for ansi_c compiler
*
* DESC:		EECS 337 Assignment 3
*
* AUTHOR:	mws85	
*
* DATE:		September 17, 2013
*
* EDIT HISTORY:	
*
*******************************************************************************/
#ifndef	YYSTYPE_H
#define	YYSTYPE_H	1

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	"y.tab.h"
/*
 *	define a global data structure
 */
#define	DATA	struct data
DATA
{
	int	column;
	int	flags;
#define	FLAGS_ECHO	0x0001
#define	FLAGS_DEBUG	0x0002
#define	FLAGS_PARSE	0x0004
#define	IS_FLAGS_ECHO(a)	(a & FLAGS_ECHO)	
#define	SET_FLAGS_ECHO(a)	(a |= FLAGS_ECHO)
#define	CLR_FLAGS_ECHO(a)	(a &= ~FLAGS_ECHO)
#define	IS_FLAGS_DEBUG(a)	(a & FLAGS_DEBUG)	
#define	SET_FLAGS_DEBUG(a)	(a |= FLAGS_DEBUG)
#define	CLR_FLAGS_DEBUG(a)	(a &= ~FLAGS_DEBUG)
#define	IS_FLAGS_PARSE(a)	(a & FLAGS_PARSE)	
#define	SET_FLAGS_PARSE(a)	(a |= FLAGS_PARSE)
#define	CLR_FLAGS_PARSE(a)	(a &= ~FLAGS_PARSE)
};

/*
 *	define for yyparser debugging
 */
#define	YYDEBUG	1
/*
 *	external variables and functions from scan.l
 */
extern FILE	*yyin;
extern FILE	*yyout;
extern	char	*yytext;
//extern	YYSTYPE yylval;
extern	int	yywrap( void);
extern	void	comment( void);
extern	void	count(  int token);
extern	int	check_type( void);
extern	void	commentdouble( void);
/*
 *	external variables and functions from gram.y
 */
extern	int	yydebug;
extern	int	yyparse( void);
extern	void	yyerror( char *s);
/*
 *	external variables and functions from main.c
 */
extern	DATA	data;
extern	int	main( int argc, char *argv[]);

#endif
